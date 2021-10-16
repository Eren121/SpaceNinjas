#pragma once

#include "Lua.hpp"

/// @brief RAII Guard to ensure the stack size does not change after some operations.
/// @details The stack size should be the same on constructor of LuaGuard and on its destructor.
/// If it is not the same, an assertion is thrown.
class LuaGuard
{
public:
    explicit LuaGuard(lua_State *L);
    ~LuaGuard();

private:
    lua_State *m_L;

    /// @brief Top stack index on constructor
    int m_top;
};