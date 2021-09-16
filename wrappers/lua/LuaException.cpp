#include "LuaException.hpp"
#include <utility/Str.hpp>

lua_utils::LuaException::LuaException(const std::string& msg, const nostd::source_location& location)
    : ::Exception(Str{} << "Lua Exception: " << msg, location)
{

}
