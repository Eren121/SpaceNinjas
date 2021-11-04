#pragma once

#include "stage/DataBody.hpp"

namespace SpaceNinja
{
    /// @brief Entity has hitpoints
    /// @details "Living" do not means in the strict sense, it is a ship probably.
    /// But we don't use the name Ship as it can be anything : mine, destructible rock, etc...
    struct Living
    {
        float hitPoints{100.0f};

        /// @brief Remove hitPoints, and destroy entity if hitPoints hits zero.
        void damage(float amount, entt::handle handle)
        {
            hitPoints -= amount;
            if(hitPoints <= 0.0f)
            {
                handle.destroy();
            }
        }
    };
}