#include <snk/version.hpp>
#include <sstream>

namespace Snow
{
    std::string to_string(const Version& version)
    {
        std::ostringstream ss;
        ss << "v";
        ss << static_cast<int>(version.major);
        ss << "." << static_cast<int>(version.minor);
        ss << "." << static_cast<int>(version.patch);

        return ss.str();
    }
}