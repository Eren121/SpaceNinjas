#include "wrappers/lua/LuaException.hpp"
#include <snk/Str.hpp>

namespace lua_utils
{
    LuaException::LuaException(lua_State *L, const std::string &msg,
                               const nostd::source_location &location)
        : Exception(Str{} << "Lua Exception: " << getLocation(L) << ": " << msg, location)
    {
    }

    std::string LuaException::getLocation(lua_State *L, int level)
    {
        if(!L)
        {
            return "<null>";
        }

        lua_Debug ar;

        if(!lua_getstack(L, level, &ar))
        {
            return "<Invalid stack index>";
        }
        else
        {
            lua_getinfo(L, "nSl", &ar);

            if(std::string{"C"} == ar.what)
            {
                // We are inside a C function,
                // Get information about the calling level
                return getLocation(L, level + 1);
            }
            else
            {
                const int line = ar.currentline;
                return Str{} << "line " << line;
            }
        }
    }
}