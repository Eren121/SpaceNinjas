#pragma once

#include "gameplay/CollisionManager.hpp"
#include "wrappers/box2d/World.hpp"
#include "wrappers/box2d/DebugDraw.hpp"
#include "media/scene/SceneNode.hpp"
#include "utility/time/Clock.hpp"
#include "utility/time/FPSCounter.hpp"
#include "utility/logging.hpp"
#include "Fwd.hpp"

class StageWorld : public b2::World, public SceneNode, public Loggable<"StageWorld">
{
public:
    explicit StageWorld(SpaceNinja::Stage& stage, Time delta = Time::seconds(1.0f / 60.0f));

    Time getDefaultDelta() const { return m_defaultDelta; }
    Clock& getClock() { return m_sinceStart; }
    const Clock& getClock() const { return m_sinceStart; }

protected:
    bool updateNode() override;
    void drawNode(RenderStates states) const override;
    void debugNode() override;

private:
    void initCollisions();
    
    /// @brief Draw a legend to know how much pixel is 1 meter
    void drawLegend(RenderStates states) const;

    SpaceNinja::Stage& getStage();
    SpaceNinja::Game& getGame();
    const SpaceNinja::Stage& getStage() const;
    const SpaceNinja::Game& getGame() const;

    SpaceNinja::Stage& m_stage;

    /// @brief Clock to synchronise the simulation.
    /// @details
    ///     Defines the speed of the simulation.
    ///     Can also pause the simulation when paused.
    ///     If not paused or scaled,
    ///     represents IRL Time elapsed since the first iteration.
    ///     But it can run faster or slowler (with Clock::setSpeed())
    ///     to have a different speed but still a fixed timestep for
    ///     more stable simulation than variable timestep.
    Clock m_sinceStart;

    /// @brief Set to true in the first update, and stay true afterwards.
    bool m_firstIterationReached{false};

    /// @brief Set to true to debug draw Box2D's physics.
    bool m_debugDrawEnabled{false};

    /// @brief Implementation of Box2D DebugDraw.
    b2::DebugDraw m_debugDraw;

    /// @brief Bodies collision management based on their types.
    /// @details
    ///     Can register callbacks when specific body types collides.
    ///     Can also disable collision between specific body types.
    CollisionManager m_collisionner;

    /// @brief The default timestep to know which value to reset with in the debug window.
    /// @details
    ///     The timestep should never be changed.
    ///     To make the simulation run faster, call more step() instead of set the delta.
    Time m_defaultDelta;

    /// @brief Counter for Simulation steps per second (Should by 1/m_delta when no cpu lag), for debugging.
    FPSCounter m_stepsPerSec;
};