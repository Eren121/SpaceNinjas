#include "stage/player/PlayerSceneNode.hpp"
#include "Game.hpp"

namespace SpaceNinja
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