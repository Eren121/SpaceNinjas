#pragma once

#include "RenderStates.hpp"

/// @brief Specify that a class has a graphical representation and can be drawn to the screen.
class Drawable
{
public:
    virtual ~Drawable() = default;
    
    /// @brief Draw this on the screen using the specified states.
    virtual void draw(RenderStates states) const = 0;
};

