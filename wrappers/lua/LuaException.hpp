#pragma once

#include "wrappers/lua/Lua.hpp"

#include <snk/exception.hpp>

namespace lua_utils
{
    class LuaException : public snk::exception
    {
    public:
        explicit LuaException(lua_State *L, const std::string& msg = "", const snk::source_location& location = snk::source_location::current());

        /// @returns The line of the current execution
        static std::string getLocation(lua_State *L, int level = 0);
    };
}
