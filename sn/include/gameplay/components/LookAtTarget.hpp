#pragma once

#include "stage/DataBody.hpp"

namespace sn
{
    /// @brief Component to look at a target.
    /// @details
    ///     Make the entity face another target entity.
    ///     The component is removed if the target die.
    ///     The entity can always face the target, or restrict the angular speed,
    ///     like for a slow machine that can't rotate very fast.
    ///     It only set the angle, not the position of the body.
    struct LookAtTarget
    {
        DataBody target;

        /// @brief At what maximum angular speed the body can turn.
        /// @details Maximum amount of radians the body can turn each tick.
        float angularSpeed{10000.0f};
    };
}