#include "gameplay/lua/LuaEngine.hpp"
#include "gameplay/lua/Thread.hpp"
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
    createMathTypes(L);

    // Set the global variable 'api' which will contain a reference to the per-Stage API instance.
    // Link to the script BY REFERENCE by linking &m_api (by pointer) and not m_api (by value => copy)
    L["api"] = &m_api;
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

    m_scripts.push_back(std::make_shared<Thread>(L, 1));
}

void SpaceNinja::script::LuaEngine::onStep()
{
    if(m_api.getIteration() == 1)
    {
        // Run the main stage coroutine

        try
        {
            startMainFunction();
        }
        catch(const lua_utils::LuaException& e)
        {
            getLogger().error(e.what());
            m_api.defeat();
        }
    }

    std::erase_if(m_scripts,
        [](auto& script) {
            return !script->isRunning();
        }
    );

    for(auto& script : m_scripts)
    {
        try
        {
            script->run();
        }
        catch(const lua_utils::LuaException& e)
        {
            getLogger().error(e.what());
            m_api.defeat();
        }
    }
}

void SpaceNinja::script::LuaEngine::addScript(std::shared_ptr<Thread>& script)
{
    m_scripts.push_back(script);
}

void SpaceNinja::script::LuaEngine::createMathTypes(sol::state &L)
{
    using namespace glm;

    vec2{} + vec2{};
    auto vec2_type = L.new_usertype<vec2>("vec2",
        sol::constructors<
            glm::vec2(),
            glm::vec2(float),
            vec2(float, float)
        >()
    );

    vec2_type["x"] = &vec2::x;
    vec2_type["y"] = &vec2::y;

    vec2_type[sol::meta_function::to_string] = static_cast<std::string(*)(const vec2&)>(&glm::to_string);

    vec2_type[sol::meta_function::addition] = sol::overload(
        static_cast<vec2(*)(const vec2&, const vec2&)>(&glm::operator+),
        static_cast<vec2(*)(float, const vec2&)>(&glm::operator+),
        static_cast<vec2(*)(const vec2&, float)>(&glm::operator+));

    vec2_type[sol::meta_function::subtraction] = sol::overload(
        static_cast<vec2(*)(const vec2&, const vec2&)>(&glm::operator-),
        static_cast<vec2(*)(float, const vec2&)>(&glm::operator-),
        static_cast<vec2(*)(const vec2&, float)>(&glm::operator-));

    vec2_type[sol::meta_function::multiplication] = sol::overload(
        static_cast<vec2(*)(const vec2&, const vec2&)>(&glm::operator*),
        static_cast<vec2(*)(float, const vec2&)>(&glm::operator*),
        static_cast<vec2(*)(const vec2&, float)>(&glm::operator*));

    vec2_type[sol::meta_function::division] = sol::overload(
        static_cast<vec2(*)(const vec2&, const vec2&)>(&glm::operator/),
        static_cast<vec2(*)(float, const vec2&)>(&glm::operator/),
        static_cast<vec2(*)(const vec2&, float)>(&glm::operator/));

    vec2_type["normalize"] = normalize<2, float, glm::defaultp>;

    UNUSED(vec2_type);
}