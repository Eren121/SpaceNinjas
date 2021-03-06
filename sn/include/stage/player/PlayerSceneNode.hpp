#pragma once

#include "stage/StageSceneNode.hpp"

class b2Body;

namespace sn
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