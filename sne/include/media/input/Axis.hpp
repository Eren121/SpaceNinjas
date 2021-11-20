#pragma once

#include <SDL_events.h>
#include <glm/vec2.hpp>

namespace Snow::in
{
/// @brief Represent an input that is represented by a 2D vector.
/// @details
/// A key can be represented by each direction.
/// @remarks If two opposite key are down, the keys cancel each other and the value is zero.
class Axis
{
public:

    /// @brief Poll the value depending of the currently pressed keys.
    glm::ivec2 poll() const;

    /// @brief Poll the value depending of the event
    /// @details
    /// If it is a key press, return the direction of the key press.
    glm::ivec2 poll(const SDL_Event &event) const;
};
}
