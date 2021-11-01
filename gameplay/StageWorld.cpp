#include "Game.hpp"
#include "gameplay/StageWorld.hpp"
#include "gameplay/Body.hpp"
#include <imgui.h>

StageWorld::StageWorld(SpaceNinja::Stage& stage, Time delta)
    : m_stage(stage),
      m_defaultDelta(delta)
{
    m_delta = m_defaultDelta;

    SetDebugDraw(&m_debugDraw);
    
    this->deleter = [](Body *userData) {
        delete userData;
    };
    
    initCollisions();
}

SpaceNinja::Stage& StageWorld::getStage() { return m_stage; }
SpaceNinja::Game& StageWorld::getGame() { return m_stage.getGame(); }
const SpaceNinja::Stage& StageWorld::getStage() const { return m_stage; }
const SpaceNinja::Game& StageWorld::getGame() const { return m_stage.getGame(); }

void StageWorld::initCollisions()
{
    m_collisionner.onEvent(Body::PlayerMissile, Body::Enemy, CollisionManager::Begin,
                    [this](b2Body &missile, b2Body &enemy) {
        // Kill enemy with shots
        //markForDestroy(&missile);
        markForDestroy(&enemy);
    });

    m_collisionner.onEvent(Body::Player, Body::Enemy, CollisionManager::Begin,
                           [this](b2Body &player, b2Body &) {
        // Kill player on collision with enemies
        markForDestroy(&player);
    });

    m_collisionner.onEvent(Body::Universe, CollisionManager::End, [this](b2Body&, b2Body& any) {

        Body::Type type{Body::None};
        if(any.GetUserData()) type = any.GetUserData()->type;

        getLogger().trace("Destroyed body out of bounds, type={}", type);

        markForDestroy(&any);
    });

    m_collisionner.setCollisionEnabled(Body::PlayerLimits, Body::Enemy, false);
    m_collisionner.setCollisionEnabled(Body::PlayerLimits, Body::PlayerMissile, false);
    m_collisionner.setCollisionEnabled(Body::Player, Body::PlayerMissile, false);
    m_collisionner.setCollisionEnabled(Body::Enemy, Body::Enemy, false);
    m_collisionner.setCollisionEnabled(Body::PlayerMissile, Body::PlayerMissile, false);
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

void StageWorld::drawNode(RenderStates states) const
{
    states.view =  getStage().getViewMatrix();

    for(const auto& b2body : *this)
    {
        const Body *body = b2body.GetUserData();

        if (body)
        {
            // If there is user data
            body->draw(states);
        }
    }
    
    if(m_debugDrawEnabled)
    {
        m_debugDraw.states = states;
        const_cast<StageWorld*>(this)->DebugDraw();

        drawLegend(states);
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
