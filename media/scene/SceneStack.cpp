#include "SceneStack.hpp"

SceneStack::SceneStack()
{
    m_stack.push(std::make_shared<SceneNode>());
}

void SceneStack::pop()
{
    m_pendings.push([](SceneStack& self) {
        
        self.m_stack.pop();
    });
}

void SceneStack::push(std::shared_ptr<SceneNode> node)
{
    m_pendings.push([node=std::move(node)](SceneStack& self) {
        self.m_stack.push(node);
    
    
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
