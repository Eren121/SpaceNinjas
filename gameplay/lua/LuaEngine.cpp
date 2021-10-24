#include "gameplay/lua/LuaEngine.hpp"
#include "gameplay/lua/ProcessCoroutine.hpp"
#include "gameplay/lua/LuaAPI.hpp"
#include "gameplay/Stage.hpp"
#include "wrappers/lua/LuaGuard.hpp"
#include "wrappers/lua/LuaException.hpp"
#include "utility/macro/unused.hpp"

SpaceNinja::script::LuaEngine::LuaEngine(Stage& stage)
    : StageSceneNode(stage), m_api(stage)
{
    L.open_libraries(
        sol::lib::base,
        sol::lib::table,
        sol::lib::math,
        sol::lib::io,
        sol::lib::coroutine,
        sol::lib::package
    );
    
    // Register the 'LuaAPI' class in Lua.
    SpaceNinja::script::LuaAPI::createNewType(L);

    // Set the global variable 'api' which will contain a reference to the per-Stage API instance.
    // Link to the script BY REFERENCE by linking &m_api (by pointer) and not m_api (by value => copy)
    L["api"] = &m_api;

    // Run the main stage coroutine
    startMainFunction();
}

void SpaceNinja::script::LuaEngine::startMainFunction()
{
    LuaGuard guard(L);
    
    // Run the main script file that should contain a function void main(int stageID);
    auto code = L.script_file("../assets/scripts/stage.lua");

    if(!code.valid())
    {
        sol::error err = code;
        std::string msg = Str{} << "Script error: " << err.what();

        throw lua_utils::LuaException(L, msg);
    }

    // Run void main(int stageID); in m_stageCoro
    lua_getglobal(L, "main");
    lua_pushinteger(L, getStage().getID());
    m_stageCoro = std::make_shared<ProcessCoroutine>(L, 1);
}

void SpaceNinja::script::LuaEngine::onStep()
{
    m_stageCoro->run();
}

void SpaceNinja::script::LuaEngine::createMathTypes(sol::state &L)
{
    ///TODO
    /*auto vec2 = L.new_usertype<glm::vec2>("vec2");
    vec2["x"] = &glm::vec2::x;
    vec2["y"] = &glm::vec2::y;*/

   //UNUSED(vec2);
}
