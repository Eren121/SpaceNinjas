#pragma once

#include "stage/DataBody.hpp"

namespace sn
{
    struct Missile
    {
        /// @brief Used so entity can't collide with their own missiles
        /// @details It's easier because getting body size in Box2D is quite complicated
        /// we would need to iterate each fixture.
        DataBody thrower;

        /// @brief How much damage the missile will do.
        float power;
    };
}