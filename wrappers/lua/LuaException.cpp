#include "wrappers/lua/LuaException.hpp"
#include "utility/Str.hpp"

namespace lua_utils
{
    LuaException::LuaException(lua_State *L, const std::string &msg,
                               const nostd::source_location &location)
        : Exception(Str{} << "Lua Exception: " << getLocation(L) << ": " << msg, location)
    {
    }

    std::string LuaException::getLocation(lua_State *L)
    {
        if(!L)
        {
            return "<null>";
        }

        lua_Debug ar;

        if(!lua_getstack(L, 0, &ar))
        {
            return "<Invalid stack index>";
        }
        else
        {
            lua_getinfo(L, "nSl", &ar);
            const int line = ar.currentline;

            return Str{} << "line " << line;
        }
    }
}