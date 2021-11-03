#pragma once

#include "formatter.hpp"

namespace snk
{
    /// @brief Generic version to_string - compatible to help printing library versions.
    struct version
    {
        explicit version(int major = 0, int minor = 0, int patch = 0)
            : major(major), minor(minor), patch(patch) {}

        int major;
        int minor;
        int patch;
    };

    std::string to_string(const version& v);
}

template<>
struct fmt::formatter<snk::version>
    : snk::formatter<snk::version> {};