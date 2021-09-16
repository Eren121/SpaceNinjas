#pragma once

#include "SceneNode.hpp"
#include <stack>
#include <queue>
#include <functional>

/// @brief Scene Stack to manage transitions between Scenes.
/// @remarks Also one feature is to supports erasing and adding new Scenes into a Scene method.
/// To manage it without memory error, they are added to pending processes that are only
/// executed at the end of the Scene update. Otherwise, a Scene could delete itself before
/// the end of its update() function if it popped itself for example to replace it by another Scene.
class SceneStack
{
public:
    /// @remarks In the constructor, an empty Scene is pushed to prevent operator->() to return nullptr.
    /// Unless the bottom Scene is popped, the stack is never empty.
    SceneStack();
    
    /// @name
    /// @brief Get the top Scene.
    /// @{
    std::shared_ptr<SceneNode>& operator ->() { return m_stack.top(); }
    const std::shared_ptr<SceneNode>& operator ->() const { return m_stack.top(); }
    /// @}
    
    /// @name
    /// @{
    /// @remarks These operations are not immediately taken into consideration, to prevent a Scene removing
    /// itself for example.
    /// You must call flush() to apply these changes.
    void pop();
    void push(std::shared_ptr<SceneNode> node);
    /// @}
    
    void update(bool printDebugInfo);
    
    const auto& getContainer() const { return m_stack; }
    
private:
    void flush();
    
    std::stack<std::shared_ptr<SceneNode>> m_stack;
    
    /// @brief Pending operations (pops/pushes).
    /// @remarks Argument should be *this.
    std::queue<std::function<void(SceneStack&)>> m_pendings;
};

