#include "Key.hpp"
#include <stdexcept>

namespace Snow::in
{
bool isKeyPressed(const Key &key)
{
    switch (key.index()) {
        case 0: {
            const auto *keyboard = SDL_GetKeyboardState(nullptr);
            return keyboard[SDL_GetScancodeFromKey(std::get<SDL_Keycode>(key))];
        }

        case 1: {
            SDL_GameController *controller = SDL_GameControllerFromPlayerIndex(0);
            return controller && SDL_GameControllerGetButton(controller, std::get<SDL_GameControllerButton>(key));
        }

        default:
            throw std::runtime_error("Unhandled std::variant");
    }
}
}