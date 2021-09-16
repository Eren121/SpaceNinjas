#pragma once

#include "PlayerProcess.hpp"
#include <SDL2/SDL_keycode.h>

class Game;
class Window;

/// @brief Manages the movement of the player with input (keyboard, mouse, etc...).
class PlayerControl : public PlayerProcess
{
public:
    using PlayerProcess::PlayerProcess;

    void tryMove(b2Body& player);

    /// @brief Rotation strategy by always facing the mouse cursor
    void tryRotateFollowMouse(b2Body& player);

    /// @brief Rotation strategy by using the directional keys to rotate
    void tryRotateWithKeys(b2Body& player);

    [[nodiscard]] glm::vec2
    getDirection(SDL_Scancode left, SDL_Scancode right, SDL_Scancode up, SDL_Scancode down) const;

protected:
    void update(b2Body& player) override;

    /// @brief Force the player to stay in the window view by clamping its coordinates
    void clampPlayer(b2Body& player);

    glm::vec2 convertClipSpaceToSimulationSpace(const glm::vec2& clipCoords) const;
};

