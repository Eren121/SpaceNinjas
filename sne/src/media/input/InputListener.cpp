#include "media/input/InputListener.hpp"
#include "media/Window.hpp"

namespace Snow::in
{
InputListener::InputListener(media::Window &window)
        : m_window(window)
{
    window.onEvent.connect(&InputListener::onEvent, this);
}

void InputListener::onEvent(const SDL_Event &event, long frame)
{
    auto type = static_cast<SDL_EventType>(event.type);
    onEventType[type](Source{
            .event = event,
            .frame = frame
    });

    switch (type) {
        case SDL_KEYUP:
        case SDL_KEYDOWN: {
            const SDL_Keycode code = event.key.keysym.sym;
            const bool pressed = (event.key.state == SDL_PRESSED);

            onKeyChanged[code](code, pressed, frame);
            break;
        }

        case SDL_CONTROLLERBUTTONUP:
        case SDL_CONTROLLERBUTTONDOWN: {
            const auto button = static_cast<const SDL_GameControllerButton>(event.cbutton.button);
            const bool pressed = event.cbutton.state == SDL_PRESSED;

            onKeyChanged[button](button, pressed, frame);
            break;
        }

        case SDL_CONTROLLERAXISMOTION: {
            const auto axis = static_cast<const SDL_GameControllerAxis>(event.caxis.axis);
            const int value = event.caxis.value;
            SDL_GameController *controller = SDL_GameControllerFromInstanceID(event.caxis.which);

            onAxisChanged[axis](axis, value, frame, controller);
            break;
        }
    }
}

media::Window &InputListener::getWindow()
{
    return m_window;
}
}