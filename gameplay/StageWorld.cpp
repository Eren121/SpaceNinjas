#include "Game.hpp"
#include "gameplay/StageWorld.hpp"
#include "gameplay/Body.hpp"
#include <imgui.h>

StageWorld::StageWorld(SpaceNinja::Game& game, Time delta)
    : m_game(game),
      m_collisionner(*this),
      m_defaultDelta(delta)
{
    m_delta = m_defaultDelta;

    SetDebugDraw(&m_debugDraw);
    
    this->deleter = [](Body *userData) {
        delete userData;
    };
    
    initCollisions();
}

void StageWorld::initCollisions()
{
    m_collisionner.connect(Body::Void, [this](b2Body&, b2Body& any) {
        
        // "Garbage collect" entities that go too far away
        markForDestroy(&any);
    });
    
    m_collisionner.connect(Body::PlayerMissile, Body::Ennemy, [this](b2Body& missile, b2Body& ennemy) {
        
        // Kill ennemy with shots
        markForDestroy(&missile);
        markForDestroy(&ennemy);
    });
    
    m_collisionner.connect(Body::Player, Body::Ennemy, [this](b2Body& player, b2Body&) {
        
        // Kill player on collision with ennemies
        markForDestroy(&player);
    });
    
    m_collisionner.setCollisionEnabled(Body::Player, Body::PlayerMissile, false);
    m_collisionner.setCollisionEnabled(Body::Ennemy, Body::Ennemy, false);
    m_collisionner.setCollisionEnabled(Body::PlayerMissile, Body::PlayerMissile, false);
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

    // Never run faster than real-time simulation, but can run slower if CPU is too slow
    // And never run more than 1 step per frame in case of CPU bottleneck (if() and not while())

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
    // m_view is in meters, we want in pixels
    // m => px
    // m * (px/m) = px
    
    const glm::vec2 windowSize = m_game.getWindow().getSize();
    
    // Get the view size in meters (but based on the size of the window)
    Rect view(windowSize / m_game.getPixelPerMeter());
    view.setCenter({0.0f, 0.0f});
    
    // convert the view in [-1;1] to windowSize coordinates
    
    states.view = glm::ortho(view.left(), view.right(), view.bottom(), view.top());
    
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
        
        drawLegend(states, m_game.getPixelPerMeter());
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

void StageWorld::drawLegend(RenderStates states, float pxPerMeter) const
{
    glm::vec2 a { 0.0f, 0.0f};
    glm::vec2 b {1.0f, 0.0f};
    glm::vec4 color {0.0f, 0.0f, 1.0f, 1.0f};
    
    Line line;
    line.update(a, b, color);
    line.setPosition({10.0f, 10.0f});
    line.setScale({pxPerMeter, 1.0f});
    line.draw(states);
}
