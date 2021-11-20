#include "stage/player/PlayerSceneNode.hpp"
#include "Game.hpp"

namespace sn
{
    PlayerSceneNode::PlayerSceneNode(Stage &stage)
            : StageSceneNode{stage}
    {
    }

    void PlayerSceneNode::onStep()
    {
        if (getStage().hasPlayer())
        {
            onStep(getStage().getPlayer().getBody());
        }
    }
}