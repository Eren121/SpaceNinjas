#include "PlayerProcess.hpp"
#include "Game.hpp"

PlayerProcess::PlayerProcess(Stage& stage)
    : m_stage(stage)
{
}

bool PlayerProcess::updateNode()
{
    if(m_stage.hasPlayer())
    {
        update(m_stage.getPlayer());
    }
    
    return true;
}
