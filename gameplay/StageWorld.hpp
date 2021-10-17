#pragma once

#include "gameplay/CollisionManager.hpp"
#include "wrappers/box2d/World.hpp"
#include "wrappers/box2d/DebugDraw.hpp"
#include "media/scene/SceneNode.hpp"
#include "Fwd.hpp"

class StageWorld : public b2::World, public SceneNode
{
public:
    explicit StageWorld(SpaceNinja::Game& game);

protected:
    bool updateNode() override;
    void drawNode(RenderStates states) const override;
    void debugNode() override;
    
private:
    void initCollisions();
    
    /// @brief Draw a legend to know how much pixel is 1 meter
    void drawLegend(RenderStates states, float pxPerMeter) const;
    
    SpaceNinja::Game& m_game;
    bool m_debugDrawEnabled{false};
    b2::DebugDraw m_debugDraw;
    CollisionManager m_collisionner;
};