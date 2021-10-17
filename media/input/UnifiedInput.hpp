#pragma once

#include "InputListener.hpp"
#include <sigslot/signal.hpp>
#include <SDL_events.h>
#include <glm/vec2.hpp>
#include <map>

namespace Snow::media
{
class Window;
}

namespace Snow::in
{
/// @brief Provides the same basic features as the library 'gainput'.
class UnifiedInput : public InputListener
{
public:
    explicit UnifiedInput(media::Window &window);

    /// @brief Get the mouse position, in window coordinates, origin in top left.
    glm::vec2 getMouse() const;

    /// @brief Get the mouse position in OpenGL clip space, that is in interval [-1;1].
    /// @details
    /// Center of the window is (0, 0)
    /// Top-Left is (1, -1)
    /// Bottom-Left is (-1, -1).
    glm::vec2 getMouseInClipSpace() const;
};
}