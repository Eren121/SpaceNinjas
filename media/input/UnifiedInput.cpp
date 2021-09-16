#include "UnifiedInput.hpp"
#include "media/Window.hpp"
#include "utility/macro/unused.hpp"

UnifiedInput::UnifiedInput(Window& window)
    : InputListener(window)
{
}

glm::vec2 UnifiedInput::getMouse() const
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

glm::vec2 UnifiedInput::getMouseInClipSpace() const
{
    glm::vec2 size = m_window.getSize();

    glm::vec2 mouse = getMouse(); // mouse is in [0;size]

    mouse /= size; // mouse is in [0;1]

    mouse = mouse * 2.0f - 1.0f; // mouse is in [-1;1]

    mouse.y = -mouse.y; // We still need to reverse Y because the sens is opposite in clip space / window space.

    return mouse;
}