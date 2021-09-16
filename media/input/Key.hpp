#pragma once

#include <SDL2/SDL_events.h>
#include <variant>

/// @brief Unify gamepad buttons and keyboard buttons
using Key = std::variant<SDL_Keycode, SDL_GameControllerButton>;

bool isKeyPressed(const Key& key);