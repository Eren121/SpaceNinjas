#pragma once

#include <snk/time/Time.hpp>

namespace SpaceNinja
{
    /// @brief Component that make entity shoot on the body's direction.
    /// @remarks It does not change the body angle, or position, the missile will go in the current
    /// direction. If you want a specific target, you have to make sure the body face your target
    /// like with LookAtTarget behaviour.
    struct Turret
    {
        /// @brief When the turett last fired, to know if the turett can fire again.
        Time lastShot;

        /// @brief Shooting period
        Time period{Time::milliseconds(1400)};

        /// @brief How fast the missiles should be
        float missileSpeed{10};
    };
}