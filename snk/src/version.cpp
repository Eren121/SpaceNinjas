#include "version.hpp"
#include <sstream>

namespace snk
{
    std::string to_string(const version& version)
    {
        std::ostringstream ss;
        ss << "v";
        ss << static_cast<int>(version.major);
        ss << "." << static_cast<int>(version.minor);
        ss << "." << static_cast<int>(version.patch);

        return ss.str();
    }
}