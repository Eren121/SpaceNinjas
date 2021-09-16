#include "LuaEngine.hpp"
#include "LuaStageCoroutine.hpp"
#include "../Stage.hpp"
#include "wrappers/lua/LuaGuard.hpp"

LuaEngine::LuaEngine(Stage& stage)
    : m_stage(stage), m_api(stage)
{
    m_L.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::coroutine);
    
    // Link the LuaAPI type
    LuaAPI::createNewType(m_L);
    
    // Link the LuaAPI instance
    // Link to the script BY REFERENCE by linking &m_api (by pointer) and not m_api (by value => copy)
    m_L["api"] = &m_api;
    
    // Rune the main stage coroutine
    initMain();
}

void LuaEngine::initMain()
{
    LuaGuard guard(m_L);
    
    // Run the main script file
    m_L.do_file("../assets/scripts/stage.lua");
    
    // Push the main generator on top of the stack
    lua_getglobal(m_L, "main");
    
    // Push the argument (the level)
    lua_pushinteger(m_L, m_stage.getID());
    
    // Creates the coroutine
    m_stageCoro = std::make_shared<LuaStageCoroutine>(m_L, 1);
}

void LuaEngine::update()
{
    (*m_stageCoro)();
}
