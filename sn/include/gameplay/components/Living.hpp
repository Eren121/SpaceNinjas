#pragma once

#include "stage/DataBody.hpp"

namespace sn
{
    /// @brief Entity has hitpoints
    /// @details "Living" do not means in the strict sense, it is a ship probably.
    /// But we don't use the name Ship as it can be anything : mine, destructible rock, etc...
    struct Living
    {
        /// @remarks If it hits zero, the entity is dead.
        float hitPoints{1.0f};

        /// @brief Raw damage reduction
        float armor{0.0f};

        /// @brief Remove hitPoints, and destroy entity if hitPoints hits zero.
        void damage(float amount, DataBody& data);
    };

    /// @brief Entity that have this components are invincible and cannot be killed by collisions or missiles.
    struct Invincible
    {
    };
}