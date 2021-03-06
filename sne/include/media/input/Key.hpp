#pragma once

#include <SDL_events.h>
#include <variant>

namespace Snow::in
{
/// @brief Unify gamepad buttons and keyboard buttons
using Key = std::variant<SDL_Keycode, SDL_GameControllerButton>;

bool isKeyPressed(const Key &key);
}