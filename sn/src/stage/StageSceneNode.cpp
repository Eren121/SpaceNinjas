#include "stage/StageSceneNode.hpp"
#include "stage/Stage.hpp"

namespace sn
{
    StageSceneNode::StageSceneNode(Stage &stage)
        : m_stage(stage)
    {
    }

    bool StageSceneNode::updateNode()
    {
        const int worldIt = m_stage.getWorld().getIteration();

        // guard to not infinite loop
        if(m_lastRunIteration <= worldIt)
        {
            // while() an not if() because we could have missed multiple steps!
            // && m_running because onStep() can cancel the node

            while(worldIt != m_lastRunIteration && m_running)
            {
                m_lastRunIteration++;
                onStep();
            }
        }
        else
        {
            getLogger().error("stored iteration ({}) is bigger than world iteration ({})",
                m_lastRunIteration, worldIt);
        }

        return m_running;
    }

    Stage& StageSceneNode::getStage()
    {
        return m_stage;
    }

    const Stage& StageSceneNode::getStage() const
    {
        return m_stage;
    }

    b2::World& StageSceneNode::getWorld()
    {
        return m_stage.getWorld();
    }

    const b2::World& StageSceneNode::getWorld() const
    {
        return m_stage.getWorld();
    }
}