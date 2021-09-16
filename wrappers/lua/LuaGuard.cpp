#include "LuaGuard.hpp"
#include "Lua.hpp"
#include <cassert>

LuaGuard::LuaGuard(lua_State *L)
    : m_L(L), m_top(lua_gettop(L))
{

}

LuaGuard::~LuaGuard()
{
    assert(lua_gettop(m_L) == m_top);
}