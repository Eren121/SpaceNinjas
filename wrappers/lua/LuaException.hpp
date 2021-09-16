#pragma once

#include <utility/Exception.hpp>

namespace lua_utils
{

class LuaException : public Exception
{
public:
    explicit LuaException(const std::string& msg = "", const nostd::source_location& location = nostd::source_location::current());
};

}
