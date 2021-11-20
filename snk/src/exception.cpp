#include "exception.hpp"
#include <sstream>

snk::exception::exception(const std::string& msg, const snk::source_location& location)
    : m_location(location)
{
    std::ostringstream ss;
    ss << location << ": ";
    ss << msg;

    m_message = ss.str();
}

const snk::source_location& snk::exception::getLocation() const
{
    return m_location;
}

const char *snk::exception::what() const noexcept
{
    return m_message.c_str();
}
