#pragma once

#include <time/Time.hpp>

namespace sn
{
    /// @rief The entity can attack
    struct CanAttack
    {
        /// @brief When the entity last fired, to know if it can fire again.
        Time lastShot;

        /// @brief Shooting period
        Time period;

        /// @brief How fast the missiles should be
        float missileSpeed{};

        /// @brief How much damage the missiles should make
        float missileDamage{};
    };
}