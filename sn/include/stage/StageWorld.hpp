#pragma once

#include "CollisionManager.hpp"
#include <b2/World.hpp>
#include <b2/DebugDraw.hpp>
#include <media/scene/SceneNode.hpp>
#include <time/Clock.hpp>
#include <time/FPSCounter.hpp>
#include <logging.hpp>
#include "Fwd.hpp"

namespace sn
{
    class StageWorld : public b2::World, public SceneNode
    {

        LOGGABLE("StageWorld")

    public:
        explicit StageWorld(sn::Stage& stage, Time delta = Time::seconds(1.0f / 60.0f));

        Time getDefaultDelta() const { return m_defaultDelta; }
        Clock& getClock() { return m_sinceStart; }
        const Clock& getClock() const { return m_sinceStart; }

        /// @brief Main function to create bodies.
        /// @details
        ///     Do not use b2Body::CreateBody() to create bodies, because it will not
        ///     create the associated entity with the body.
        ///     Each entity has a body (even if it has no fixtures), and each body has an entity
        ///     to simplify lifetime management.
        ///     To destroy an entity, do not use entt::registry::destroy() nor b2World::DestroyEntity()
        ///     but markForDestroy(). The advantage is double:
        ///         1) It will clean both body and entity
        ///         2) The entity will only destroyed at the end of the step so the entity is still
        ///            safely useable in the rest in the caller function (no NPE on use).
        DataBody& createBody(b2BodyDef& def);

        /// @brief Just a wrapper for the parent function.
        void markForDestroy(DataBody& data) { b2::World::markForDestroy(&data.getBody()); }

        entt::registry& getRegistry() { return m_registry; }

        sn::Stage& getStage();
        sn::Game& getGame();
        const sn::Stage& getStage() const;
        const sn::Game& getGame() const;

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


        entt::registry m_registry;

        sn::Stage& m_stage;

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