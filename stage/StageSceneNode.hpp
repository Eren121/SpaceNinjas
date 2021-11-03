#pragma once

#include "media/scene/SceneNode.hpp"
#include <snk/logging.hpp>
#include "Fwd.hpp"

namespace SpaceNinja
{
    class StageSceneNode : public SceneNode
    {
        LOGGABLE("StageSceneNode")

    public:
        explicit StageSceneNode(Stage& stage);

    protected:
        bool updateNode() final;

        /// @remarks Since the Node is not always run each frame, it makes no sense to draw in it.
        void drawNode(RenderStates states) const final {}

        /// @brief Run only when a new simulation step is reached.
        virtual void onStep() = 0;

        Stage& getStage();
        const Stage& getStage() const;

        b2::World& getWorld();
        const b2::World& getWorld() const;

    private:
        bool m_running{true}; ///< Return value for updateNode()
        int m_lastRunIteration{0}; ///< What last iteration this Node was updated?
        Stage& m_stage;
    };
}