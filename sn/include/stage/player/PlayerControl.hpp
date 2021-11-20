#pragma once

#include "PlayerSceneNode.hpp"
#include <SDL_keycode.h>

namespace sn
{
    /// @brief Manages the movement of the player with input (keyboard, mouse, etc...).
    class PlayerControl : public PlayerSceneNode
    {
    public:
        using PlayerSceneNode::PlayerSceneNode;

    protected:
        void tryMove(b2Body& player);

        /// @brief Rotation strategy by always facing the mouse cursor
        void tryRotateFollowMouse(b2Body& player);

        /// @brief Rotation strategy by using the directional keys to rotate
        void tryRotateWithKeys(b2Body& player);

        [[nodiscard]] glm::vec2
        getDirection(SDL_Scancode left, SDL_Scancode right, SDL_Scancode up, SDL_Scancode down) const;

        void onStep(b2Body& player) override;

        glm::vec2 convertClipSpaceToSimulationSpace(const glm::vec2& clipCoords) const;
    };
}

