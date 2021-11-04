#pragma once

#include "stage/CollisionManager.hpp"
#include "wrappers/box2d/World.hpp"
#include "wrappers/box2d/DebugDraw.hpp"
#include "media/scene/SceneNode.hpp"
#include <snk/time/Clock.hpp>
#include <snk/time/FPSCounter.hpp>
#include <snk/logging.hpp>
#include "Fwd.hpp"

namespace SpaceNinja
{
    class StageWorld : public b2::World, public SceneNode
    {
        LOGGABLE("StageWorld")

    public:
        explicit StageWorld(SpaceNinja::Stage& stage, Time delta = Time::seconds(1.0f / 60.0f));

        Time getDefaultDelta() const { return m_defaultDelta; }
        Clock& getClock() { return m_sinceStart; }
        const Clock& getClock() const { return m_sinceStart; }

        /// @{
        /// @brief Body factories
        b2Body& createEnemyBody(const glm::vec2 &pos);
        b2Body& createPlayerBody(const glm::vec2 &pos);
        b2Body& createMissileBody(const glm::vec2 &pos, const DataBody& thrower);

        b2Body& createBoxBody(const glm::vec2 &pos, float width, const std::string &texture);
        using World::createBoxBody;
        /// @}

        entt::registry& getRegistry() { return m_registry; }

    protected:
        bool updateNode() override;
        void drawNode(RenderStates states) const override;
        void debugNode() override;

        void onDestroy(b2Body& body) override;

        void onCreate(b2Body& body) override;


    private:
        void initCollisions();

        /// @brief Draw a legend to know how much pixel is 1 meter
        void drawLegend(RenderStates states) const;

        SpaceNinja::Stage& getStage();
        SpaceNinja::Game& getGame();
        const SpaceNinja::Stage& getStage() const;
        const SpaceNinja::Game& getGame() const;

        entt::registry m_registry;

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
}