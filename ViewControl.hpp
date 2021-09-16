#pragma once

#include "media/Window.hpp"
#include <utility/Rect.hpp>
#include <unordered_map>
#include <functional>

/// @brief Control the view with inputs (like a camera).
class ViewControl
{
public:
    ViewControl(Window& window);

protected:
    void onEvent(const SDL_Event& event);

private:
    Window& m_window;

    std::unordered_map<SDL_Keycode, std::function<void()>> m_mappings;
};

