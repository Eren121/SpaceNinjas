#pragma once

namespace sn
{
    enum BodyType
    {
        None,
        Player,
        Enemy,
        PlayerLimits, ///< Barriers at the limit of the world, to garbage collect entities colliding with it
        Universe, ///< Everything that end collision with this body is destroyed

        PlayerMissile,

        Count ///< Do not use, to know the count of Types
    };
}