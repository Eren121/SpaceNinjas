#pragma once

#include "formatter.hpp"

namespace snk
{
    /// @brief Generic version to_string - compatible to help printing library versions.
    struct version
    {
        int major;
        int minor;
        int patch;
    };

    std::string to_string(const version& v);
}

template<>
struct fmt::formatter<snk::version>
    : snk::formatter<snk::version> {};