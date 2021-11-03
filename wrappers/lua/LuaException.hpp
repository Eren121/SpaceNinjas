#pragma once

#include "wrappers/lua/Lua.hpp"
#include <snk/Exception.hpp>

namespace lua_utils
{
    class LuaException : public Exception
    {
    public:
        explicit LuaException(lua_State *L, const std::string& msg = "", const nostd::source_location& location = nostd::source_location::current());

        /// @returns The line of the current execution
        static std::string getLocation(lua_State *L, int level = 0);
    };
}
