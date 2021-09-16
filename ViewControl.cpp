#include "ViewControl.hpp"

ViewControl::ViewControl(Window& window)
    : m_window(window)
{
    m_window.onEvent.connect([this](const SDL_Event& event, long) {
        onEvent(event);
    });

    m_mappings[SDLK_F11] = [this] {
        m_window.setFullscreen(!m_window.isFullscreen());
    };
}

void ViewControl::onEvent(const SDL_Event& event)
{
    if(event.type == SDL_KEYDOWN)
    {
        SDL_Keycode code = event.key.keysym.sym;

        if(m_mappings.contains(code))
        {
            m_mappings[code]();
        }
    }
}
