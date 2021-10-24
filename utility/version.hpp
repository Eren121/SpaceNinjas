#pragma once

#include "utility/fmt.hpp"
#include <string>

namespace Snow
{
    /// @brief Provide a POD with major/minor/patch members.
    struct Version
    {
        int major, minor, patch;
    };

    std::string to_string(const Version& version);
}

SNOW_DEFINE_USER_FMT(std::same_as<Snow::Version>);