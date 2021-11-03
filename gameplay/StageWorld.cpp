#include "Game.hpp"
#include "gameplay/StageWorld.hpp"

#include <imgui.h>

namespace SpaceNinja
{
    StageWorld::StageWorld(Stage& stage, Time delta)
        : m_stage(stage),
          m_defaultDelta(delta)
    {
        m_delta = m_defaultDelta;
        deleter = [](DataBody& data) {
            if(data.handle)
            {
                data.handle.destroy();
            }
        };

        SetDebugDraw(&m_debugDraw);

        initCollisions();
    }

    Stage& StageWorld::getStage() { return m_stage; }
    Game& StageWorld::getGame() { return m_stage.getGame(); }
    const Stage& StageWorld::getStage() const { return m_stage; }
    const Game& StageWorld::getGame() const { return m_stage.getGame(); }

    void StageWorld::initCollisions()
    {
        m_collisionner.onEvent(BodyType::PlayerMissile, BodyType::Enemy, CollisionManager::Begin,
                        [this](b2Body &missile, b2Body &enemy) {
            // Kill enemy with shots
            markForDestroy(&missile);
            markForDestroy(&enemy);
        });

        m_collisionner.onEvent(BodyType::Player, BodyType::Enemy, CollisionManager::Begin,
                               [this](b2Body &player, b2Body &) {
            // Kill player on collision with enemies
            markForDestroy(&player);
        });

        m_collisionner.onEvent(BodyType::Universe, CollisionManager::End, [this](b2Body&, b2Body& any) {

            BodyType type{any.GetUserData().type};

            getLogger().trace("Destroyed body out of bounds, type={}", type);

            markForDestroy(&any);
        });

        m_collisionner.setCollisionEnabled(BodyType::PlayerLimits, BodyType::Enemy, false);
        m_collisionner.setCollisionEnabled(BodyType::PlayerLimits, BodyType::PlayerMissile, false);
        m_collisionner.setCollisionEnabled(BodyType::Player, BodyType::PlayerMissile, false);
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

        // Using WHILE can freeze the gameif the cpu is too slow, care

        if(m_sinceStart.getElapsedTime() >= getTime())
        {
            constexpr const char *msgFormat = "pre-step {}; simulation time = {}; time since start = {}";
            const int nextIt = getIteration() + 1;
            getLogger().trace(msgFormat, nextIt, getTime(), m_sinceStart.getElapsedTime());

            step();
            m_stepsPerSec.onFrame();
        }

        return true;
    }

    void StageWorld::drawNode(RenderStates parent) const
    {
        parent.view =  getStage().getViewMatrix();

        for(const auto& b2body : *this)
        {
            RenderStates states = parent;

            const DataBody& body = b2body.GetUserData();
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
                const DataFixture& data{fixture.GetUserData()};

                if(data.sprite)
                {
                    data.sprite->draw(states);
                }
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
        const ImGuiInputTextFlags readOnly = ImGuiInputTextFlags_ReadOnly;

        if(ImGui::CollapsingHeader("StageWorld"))
        {
            int iteration = getIteration();
            float time = getTime().asSeconds();
            float fps = m_stepsPerSec.getFPS();

            ImGui::InputInt("Iteration", &iteration, 0, 0, readOnly);
            ImGui::InputFloat("Time", &time, 0.0f, 0.0f, "%.3fs", readOnly);
            ImGui::InputFloat("Steps/s", &fps, 0.0f, 0.0f, "%.3f", readOnly);

            struct Range
            {
                float min;
                float max;
            };

            static const Range deltaRange{1.0f / 300.0f, 1.0};
            static const Range timeScaleRange{0.1f, 10.0f};

            float delta_as_sec = m_delta.asSeconds();
            if(ImGui::SliderFloat("Lockstep delta", &delta_as_sec, deltaRange.min, deltaRange.max, "%.5f"))
            {
                // Modified by user
                m_delta = Time::seconds(delta_as_sec);
            }

            float time_scale = m_sinceStart.getSpeed();
            if(ImGui::SliderFloat("Time scale", &time_scale, timeScaleRange.min, timeScaleRange.max, "%.5f"))
            {
                m_sinceStart.setSpeed(time_scale);
            }

            bool paused  = m_sinceStart.isPaused();
            if(ImGui::Checkbox("Pause", &paused))
            {
                m_sinceStart.setPaused(paused);
            }

            ImGui::SameLine();
            if(ImGui::Button("Reset"))
            {
                m_delta = m_defaultDelta;
            }

            ImGui::Checkbox("Debug Draw", &m_debugDrawEnabled);
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

    b2Body &StageWorld::createMissileBody(const glm::vec2 &pos)
    {
        b2Body &body{createBoxBody(pos, 2.0f, "laser.png")};
        entt::handle handle{m_registry, m_registry.create()};
        body.GetUserData() = DataBody{handle, body, BodyType::PlayerMissile};

        body.SetBullet(true);
        return body;
    }

    b2Body &StageWorld::createEnemyBody(const glm::vec2 &pos)
    {
        b2Body &body{createBoxBody(pos, 2.0f, "enemy.png")};
        entt::handle handle{m_registry, m_registry.create()};
        body.GetUserData() = DataBody{handle, body, BodyType::Enemy};

        return body;
    }

    b2Body &StageWorld::createPlayerBody(const glm::vec2 &pos)
    {
        b2Body &body{createBoxBody(pos, 2.0f, "player.png")};
        body.SetLinearDamping(0.8f);
        entt::handle handle{m_registry, m_registry.create()};
        body.GetUserData() = DataBody{handle, body, BodyType::Player};

        return body;
    }

    b2Body& StageWorld::createBoxBody(const glm::vec2 &pos, float width, const std::string &texPath)
    {
        const Texture *texture = &m_stage.getGame().textures(texPath);
        const glm::vec2 textureSize = texture->getSize();
        const float ratio = textureSize.y / textureSize.x;

        Rect box;
        box.size = {width, width * ratio};
        box.setOriginFromCenter(pos);

        b2Body& body{World::createBoxBody(box, b2_dynamicBody, 1.0f, false)};

        // Add the drawable rectangle in center of the body with the same size as the box

        DataFixture& fixtureData{body.GetFixtureList()->GetUserData()};
        fixtureData.sprite = std::make_shared<Sprite>();
        fixtureData.sprite->setOrigin(Sprite::Centered); // sprite 0.5 / 0.5 each side
        fixtureData.sprite->setScale(box.size); // not / 2 because sprite 0.5/0.5 each side
        fixtureData.sprite->setTexture(texture);

        return body;
    }
}