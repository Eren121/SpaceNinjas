#pragma once

namespace algo
{
    /// @brief Compare pointers
    struct indirect_less
    {
        template<typename T>
        auto operator()(const T& lhs, const T& rhs) const
        {
            return *lhs < *rhs;
        }
    };
}