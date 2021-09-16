#include "Game.hpp"
#include "StageWorld.hpp"
#include "Body.hpp"
#include <imgui.h>

StageWorld::StageWorld(Game& game)
    : m_game(game),
      m_collisionner(*this)
{
    SetDebugDraw(&m_debugDraw);
    
    this->deleter = [](void *userData) {
        delete reinterpret_cast<Body*>(userData);
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
    step();
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
        const Body *body = b2::getUserData<Body>(b2body);

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
    if(ImGui::CollapsingHeader("StageWorld"))
    {
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
