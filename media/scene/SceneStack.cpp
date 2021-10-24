#include "SceneStack.hpp"

SceneStack::SceneStack()
{
    m_stack.push(std::make_shared<SceneNode>());
    m_stack.top()->onBecomeTop();
}

void SceneStack::pop()
{
    m_pendings.push([](SceneStack& self) {

        // The very bottom of the stack is an empty scene
        // Do not remove it if it is the last scene.
        // To avoid having to care of null pointers (Null Object Pattern)
        if(self.m_stack.size() > 1)
        {
            self.m_stack.pop();

            if(!self.m_stack.empty())
            {
                self.m_stack.top()->onBecomeTop();
            }
        }
    });
}

void SceneStack::push(std::shared_ptr<SceneNode> node)
{
    m_pendings.push([node=std::move(node)](SceneStack& self) {
        self.m_stack.push(node);
        
        node->onBecomeTop();
    
        // Care to not call self.pop() but self.m_stack.pop() otherwise infinite loop
    });
}

void SceneStack::update(bool printDebugInfo)
{
    if(!m_stack.top()->update(printDebugInfo))
    {
        pop();
    }
    
    flush();
}

void SceneStack::flush()
{
    while(!m_pendings.empty())
    {
        m_pendings.front()(*this);
        m_pendings.pop();
    }
}
