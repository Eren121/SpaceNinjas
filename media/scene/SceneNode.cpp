#include "SceneNode.hpp"
#include <ranges>
#include <imgui.h>

void SceneNode::draw(RenderStates states) const
{
    drawNode(states);
    
    // higher z-index will be rendered last so on top
    for(const auto& child : m_children)
    {
        child->draw(states);
    }
}

bool SceneNode::update(bool printDebugInfo)
{
    if(updateNode()) // Only update children if the node should continue
    {
        if(printDebugInfo)
        {
            debugNode();
        }
        
        // higher z-index will be updated first
        for(auto it = m_children.cbegin(); it != m_children.cend(); )
        {
            auto& child = *it;
            
            if(child->update(printDebugInfo))
            {
                // Advance
                ++it;
            }
            else
            {
                // Remove
                it = m_children.erase(it);
            }
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

void SceneNode::addChild(SceneNode::Child child, int z)
{
    child->m_z = z;
    m_children.insert(std::move(child));
}

void SceneNode::debugNode()
{
}
