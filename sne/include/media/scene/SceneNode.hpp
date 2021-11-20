#pragma once

#include <set>
#include "gl/Drawable.hpp"

/// @brief Scene graph, each node is also itself a scene graph.
class SceneNode : public Drawable
{
public:
    using Child = std::shared_ptr<SceneNode>;
    
    void addChild(Child child, int z = 0);
    
    void draw(RenderStates states) const final;
    
    /// @returns false To remove the node from its parent after its update.
    bool update(bool printDebugInfo);
    
    /// @brief Callback called when this becomes this top stack node.
    /// @details
    /// Can be called either:
    /// - When this is just pushed to the stack.
    /// - When this node was the sub-top node, and the top node is popped, making this the top node again.
    virtual void onBecomeTop() {}

    /// @brief To force to remove the node.
    /// @details When removed, update() will returns false and will not run if the node was removed externally.
    void removeNode() { m_removed = true; }

protected:
    virtual void drawNode(RenderStates states) const {}
    
    /// @returns false To remove the node from its parent after its update.
    virtual bool updateNode() { return true; }
    
    /// @brief Draw imgui stuff here if needed
    virtual void debugNode();
    
private:
    /// @brief Compare two children
    /// @details
    /// We want to store first by z-index (less first), and then by pointer value.
    struct comparator
    {
        bool operator()(const Child& lhs, const Child& rhs) const
        {
            // Compare with ordering, the first elements have more weights
            // Sort by z-index first then pointer,
            // So that elements with same z-index will not be equals
            // And therefore will not be deleted if we want to erase an element
            return std::tie(lhs->m_z, lhs) < std::tie(rhs->m_z, rhs);
        }
    };

    bool m_removed{false};
    int m_z{0}; ///< z-index
    std::set<Child, comparator> m_children;
};

