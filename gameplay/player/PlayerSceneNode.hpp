#pragma once

#include "gameplay/StageSceneNode.hpp"

namespace SpaceNinja
{
    /// @brief Node for a player behavior.
    /// @details The Process will not update if the world has not player
    class PlayerSceneNode : public StageSceneNode
    {
    public:
        explicit PlayerSceneNode(Stage &stage);

    protected:
        void onStep() final;
        virtual void onStep(b2Body &player) = 0;
    };
}