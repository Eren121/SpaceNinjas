#include "Game.hpp"
#include "stage/physics/BodyBuilder.hpp"
#include "stage/StageWorld.hpp"
#include "gameplay/components/Turret.hpp"
#include "gameplay/components/LookAtTarget.hpp"
#include "gameplay/components/Missile.hpp"
#include "gameplay/components/Living.hpp"
#include "gameplay/components/Animation.hpp"
#include "gameplay/components/Name.hpp"
#include <ft/Text.hpp>
#include <im/ImGUI.hpp>

namespace sn
{
    StageWorld::StageWorld(Stage& stage, Time delta)
        : m_stage(stage),
          m_defaultDelta(delta)
    {
        m_delta = m_defaultDelta;
        SetDebugDraw(&m_debugDraw);

        initCollisions();
    }

    Stage& StageWorld::getStage() { return m_stage; }
    Game& StageWorld::getGame() { return m_stage.getGame(); }
    const Stage& StageWorld::getStage() const { return m_stage; }
    const Game& StageWorld::getGame() const { return m_stage.getGame(); }

    void StageWorld::initCollisions()
    {
        m_collisionner.onEvent(BodyType::PlayerMissile, CollisionManager::Begin,
                        [this](b2Body &body, b2Body &any) {
            // Kill any with shots
            // ONLY if the target is not the same as the thrower
            // Otherwise missile would kill their thrower at the spawn:
            // Missile are spawned in a fixed distance from the thrower
            bool hit{true};

            if(Missile *missile = getData(body).handle.try_get<Missile>())
            {
                // We can compare
                // Even if the thrower is dead, this should be ok (no NPE)
                if(missile->thrower.handle.entity() == getData(any).handle.entity())
                {
                    // disable hit its own thrower
                    hit = false;
                }
            }
            else
            {
                hit = false;
                getLogger().warn("Tag missile without Missile component hit");
            }

            // Only hit with Living units
            /// TODO make all bodies have handle because we have for the moment to check if the user data exists
            if(!getData(any) || !getData(any).handle.try_get<Living>())
            {
                hit = false;
            }

            if(DataBody& data = getData(any))
            {
                if (Living *living = data.handle.try_get<Living>(); living && hit)
                {
                    // Damage only if hit
                    living->damage(10.0f, getData(any));

                    getLogger().trace("Damaged entity {}", Name::get(data.handle));
                }
            }

            if(hit)
            {
                markForDestroy(getData(body));
            }
        });

        m_collisionner.onEvent(BodyType::Player, BodyType::Enemy, CollisionManager::Begin,
                               [this](b2Body &player, b2Body &) {
            // Damage player on collision with enemies
           if(getData(player).handle.try_get<Living>())
           {
               Living& living = getData(player).handle.get<Living>();
               living.damage(10.0f, getData(player));
           }
        });

        m_collisionner.onEvent(BodyType::Universe, CollisionManager::End, [this](b2Body&, b2Body& any) {

            BodyType type{getData(any).type};

            getLogger().trace("Destroyed body out of bounds, type={}", type);

            markForDestroy(getData(any));
        });

        m_collisionner.setCollisionEnabled(BodyType::PlayerLimits, BodyType::Enemy, false);
        m_collisionner.setCollisionEnabled(BodyType::PlayerLimits, BodyType::PlayerMissile, false);
        //m_collisionner.setCollisionEnabled(BodyType::Player, BodyType::PlayerMissile, false);
        m_collisionner.setCollisionEnabled(BodyType::Enemy, BodyType::Enemy, false);
        m_collisionner.setCollisionEnabled(BodyType::PlayerMissile, BodyType::PlayerMissile, false);
        SetContactFilter(&m_collisionner);
        SetContactListener(&m_collisionner);
    }

    bool StageWorld::updateNode()
    {
        if(!m_firstIterationReached)
        {
            const Time elapsed = m_sinceStart.getElapsedTime();
            getLogger().debug("First update; loading time = {}", elapsed);

            m_sinceStart.restart();
            m_firstIterationReached = true;
        }

        // When using IF statement:
        //      Never run faster than real-time simulation, but can run slower if CPU is too slow
        //      And never run more than 1 step per frame in case of CPU bottleneck (if() and not while())

        // Using WHILE can freeze the game if the cpu is too slow, care

        while(m_sinceStart.getElapsedTime() >= getTime())
        {
            constexpr const char *msgFormat = "pre-step {}; simulation time = {}; time since start = {}";
            const int nextIt = getIteration() + 1;
            getLogger().trace(msgFormat, nextIt, getTime(), m_sinceStart.getElapsedTime());

            step();
            m_stepsPerSec.onFrame();

            {
                // Look at IA
                auto view = m_registry.view<LookAtTarget, b2Body*>();
                view.each([this](auto entity, LookAtTarget& lookAt, b2Body* body) {

                    if(!lookAt.target)
                    {
                        // Remove the target if it is dead
                        m_registry.remove<LookAtTarget>(entity);
                    }
                    else
                    {
                        const b2Body& target = lookAt.target.getBody();
                        const float currentAngle{b2::getAngle(*body)};

                        // Angle if the body face immediately the target
                        const float finalAngle{math::vec2angle(b2::getPosition(target) - b2::getPosition(*body))};

                        // The (signed) distance, we limit from the maximum angular speed per tick
                        float distance{math::normalize(finalAngle - currentAngle)};
                        if(std::abs(distance) > lookAt.angularSpeed)
                        {
                            distance = math::sgn(distance) * lookAt.angularSpeed;
                        }

                        b2::setAngle(*body, currentAngle + distance);
                    }
                });
            }
            {
                // Turret IA
                auto view = m_registry.view<Turret, b2Body*>();

                view.each([this](auto entity, Turret& turret, b2Body* body) {

                    const Time now{getTime()};

                    if(now >= turret.lastShot + turret.period)
                    {
                        turret.lastShot = now;

                        glm::vec2 direction{b2::getDirection(*body)};
                        glm::vec2 missilePos{b2::getPosition(*body) + direction * 3.0f};
                        DataBody& missileBody = createMissileBody(*this, missilePos, getData(*body));
                        missileBody.setVelocityWithAngle(direction * turret.missileSpeed);
                    }
                });
            }

            auto animations_view = m_registry.view<Animation>();

            for(auto&& [entity, animation] : animations_view.each())
            {
                if(getTime() >= animation.endTime)
                {
                    // Remove the animation if the time is elapsed
                    m_registry.destroy(entity);
                }

                // Update the frame accordingly to the time of the world

                // Get the texture from the path
                // The filename in the .json sprite sheet should be
                // in the same folder as all textures...
                const Time elapsed(getTime() - animation.startTime);
                const Snow::media::ase::Frame& frame{animation.sheet->getFrameFromTime(elapsed)};

                if(frame.index != animation.currentFrame)
                {
                    animation.currentFrame = frame.index;

                    const Texture& texture{getStage().getGame().textures(frame.filename)};
                    animation.sprite.setTexture(&texture);

                    Rect uvs(frame.rect);
                    uvs /= texture.getSize();

                    animation.sprite.setUVs(uvs);
                }
            }
        }

        return true;
    }

    void StageWorld::drawNode(RenderStates parent) const
    {
        parent.view =  getStage().getViewMatrix();

        for(const auto& b2body : *this)
        {
            RenderStates states = parent;

            const DataBody& body = getData(b2body);
            if(!body) continue;

            const glm::vec2 bodyPos{b2::getPosition(b2body)};
            const float bodyAngle{b2::getAngle(b2body)};

            // Apply the body's transform to the model matrix

            // Move the model to the position of the body
            states.model = glm::translate(states.model, {bodyPos, 0});

            // Rotate the model to the rotation of the body
            // OpenGL is right handed in clip space (visible only in range [-1;1])
            // +z -> towards you -> rotation in CCW order (box2D use radians)
            static const glm::vec3 axis = {0.0f, 0.0f, 1.0f};
            states.model = glm::rotate(states.model, bodyAngle, axis);

            for(b2Fixture& fixture : body)
            {
                const DataFixture& data{getData(fixture)};

                if(data.sprite)
                {
                    data.sprite->draw(states);

                    if(Living *living = body.handle.try_get<Living>())
                    {
                        // Draw health count for living bodies
                        /*
                        Text text;
                        text.setScale(glm::vec2{1.0f / 32.0f});
                        text.setString(fmt::format("{}", living->hitPoints));
                        text.setFont(&m_stage.getGame().getFont());

                        text.draw(states);
                        */
                    }
                }
            }
        }


        {
            // Draw animations

            auto animations_view = m_registry.view<Animation>();

            for(auto&& [entity, animation] : animations_view.each())
            {
                animation.sprite.draw(parent);
            }
        }

        if(m_debugDrawEnabled)
        {
            m_debugDraw.states = parent;
            const_cast<StageWorld*>(this)->DebugDraw();

            drawLegend(parent);
        }
    }

    void StageWorld::debugNode()
    {
        if(ImGui::CollapsingHeader("StageWorld"))
        {
            static ImInt iteration = ImInt::make()
                    .setReadonly()
                    .setLabel("Current iteration")
                    .setTooltip("The current step for the simulation.\nThe simulation advance a fixed amount of seconds each step.");
            iteration.value = getIteration();
            iteration.show();

            static ImTime time = ImTime::make()
                    .setReadonly()
                    .setLabel("Current time")
                    .setTooltip("The current amount of seconds elapsed.");
            time.value = getTime();
            time.show();

            static ImFloat stepsPerSec = ImFloat::make()
                    .setReadonly()
                    .setLabel("Current count of steps per second")
                    .setSuffix("steps/s")
                    .setTooltip("How many steps are performed each second.\nIf the simulation runs smoothly,\nthis must be constant.");
            stepsPerSec.value = m_stepsPerSec.getFPS();
            stepsPerSec.show();

            static ImTime lockStep = ImTime::make()
                    .setUnit(ImTime::Millisecond)
                    .setDefault(b2::World::DEFAULT_DELTA)
                    .setRange(Time::seconds(1.0f / 300.0f), Time::seconds(1.0f))
                    .setLabel("Lockstep")
                    .setTooltip("The amount of time to advance each fixed step.\n"
                                "This should not normally be changed, if not for debugging.\n"
                                "To change the time speed, rather modify the parameter \"Time scale\".");
            lockStep.value = m_delta;
            if(lockStep.show())
            {
                m_delta = lockStep.value;
            }

            static ImFloat timeScale = ImFloat::make()
                    .setDefault(1.0f)
                    .setRange(0.1f, 10.0f)
                    .setLabel("Time scale")
                    .setTooltip("How fast the time elapses.\nA higher value means that the time passes more quickly.");
            timeScale.value = m_sinceStart.getSpeed();
            if(timeScale.show())
            {
                m_sinceStart.setSpeed(timeScale.value);
            }

            static ImInt entityCount = ImInt::make()
                    .setReadonly()
                    .setLabel("Count of entities")
                    .setTooltip("The world is made of entities, that can sometimes be invisible.\nThis is a good number to check if there are lags.");
            entityCount.value = m_registry.alive();
            entityCount.show();

            static ImBool paused = ImBool::make()
                    .setLabel("Pause");
            paused.value = m_sinceStart.isPaused();
            if(paused.show())
            {
                m_sinceStart.setPaused(paused.value);
            }

            static ImBool debugDraw = ImBool::make()
                    .setLabel("Debug draw")
                    .setTooltip("Enable Box2D DebugDraw.\nThe fixtures and forces will be displayed.");
            if(debugDraw.show())
            {
                m_debugDrawEnabled = debugDraw.value;
            }
        }
    }

    void StageWorld::drawLegend(RenderStates states) const
    {
        // Since we are in world coordinates (meter), we just draw 1 unit (1 meter)...

        // We define the position in NDC, for the position of the line to be independant from the view
        const glm::vec2 p1_ndc{-0.9f, -0.9f};

        glm::vec2 p1{glm::inverse(states.view) * glm::vec4{p1_ndc, 0.0f, 1.0f}};
        glm::vec2 p2 {p1 + glm::vec2{1.0f, 0.0f}};

        glm::vec4 color {0.0f, 0.0f, 1.0f, 1.0f};

        Line line;
        line.update(p1, p2, color);
        line.draw(states);
    }

    void StageWorld::onDestroy(b2Body &body)
    {
        // Destroy the entity associated with the body
        // If you remove only from the registry, the the body will still exists on the world.
        // But destroying the body from the world will also remove the entity.

        DataBody data = getData(body);

        if(data) // Should always be true
        {
            // Prevent loop
            data.handle.template remove<b2Body*>();


            data.handle.destroy();
        }
        else
        {
            getLogger().warn("A body was destroyed which had no associated entity");
        }
    }

    void StageWorld::onCreate(b2Body &body)
    {
        // Create the associated entity to the body
        // The type will be None, it should be initialized by the function that create the entity

        const auto entity{m_registry.create()};
        const entt::handle handle{m_registry, entity};

        // Initialize the user data
        // (Note: the DataBody exists at this point but it default-constructed)
        getData(body) = DataBody{handle, body, BodyType::None};

        // Add the body component to the entity associated to the body
        getData(body).handle.emplace<b2Body*>(&body);
    }

    DataBody& StageWorld::createBody(b2BodyDef& def)
    {
        b2Body& body = b2::World::createBody(def);
        return getData(body);
    }
}