#pragma once

#include <media/ase/SpriteSheet.hpp>
#include <gl/Sprite.hpp>

namespace sn
{
    /// @brief Generic component to draw and update each frame (not each step).
    /// @details Animations don't need always physical body, so it may not have any b2Body* component.
    struct Animation
    {
        Sprite sprite;
        const Snow::SpriteSheet* sheet{nullptr};

        /// @brief The end time of the animation (in stage units)
        Time endTime;
        Time startTime;

        /// @brief To know if we have to update the frame index.
        /// Starts at -1 so the first update always update.
        int currentFrame{-1};
    };
}