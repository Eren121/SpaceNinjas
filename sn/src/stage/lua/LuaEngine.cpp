#include "stage/lua/LuaEngine.hpp"
#include "stage/Stage.hpp"
#include <lua/LuaGuard.hpp>
#include <lua/LuaException.hpp>
#include <unused.hpp>

namespace sn::script {
    LuaEngine::LuaEngine(Stage &stage)
            : StageSceneNode(stage), m_api(stage) {
        L.open_libraries(
                sol::lib::base,
                sol::lib::table,
                sol::lib::math,
                sol::lib::io,
                sol::lib::coroutine,
                sol::lib::package
        );

        // Register the 'LuaAPI' class in Lua.
        LuaAPI::createNewType(L);
        createMathTypes(L);
        createBodyType(L);
        createRectType(L);

        // Set the global variable 'api' which will contain a reference to the per-Stage API instance.
        // Link to the script BY REFERENCE by linking &m_api (by pointer) and not m_api (by value => copy)
        L["api"] = &m_api;
    }

    void LuaEngine::startMainFunction() {
        LuaGuard guard(L);

        // Run the main script file that should contain a function void main(int stageID);
        auto code = L.script_file("../assets/scripts/main.lua");

        if (!code.valid()) {
            sol::error err = code;
            std::string msg = snk::str{} << "Script error: " << err.what();

            throw lua_utils::LuaException(L, msg);
        }

        // Run void main(int stageID); in m_stageCoro
        lua_getglobal(L, "main");
        lua_pushinteger(L, getStage().getID());

        m_scripts.push_back(std::make_shared<Thread>(L, 1));
    }

    void LuaEngine::onStep() {
        if (m_api.getIteration() == 1) {
            // Run the main stage coroutine

            try {
                startMainFunction();
            }
            catch (const lua_utils::LuaException &e) {
                getLogger().error(e.what());
                m_api.triggerLoss();
            }
        }

        std::erase_if(m_scripts,
                      [](auto &script) {
                          return !script->isRunning();
                      }
        );

        for (auto &script : m_scripts) {
            try {
                script->run();
            }
            catch (const lua_utils::LuaException &e) {
                getLogger().error(e.what());
                m_api.triggerLoss();
            }
        }
    }

    void LuaEngine::addScript(std::shared_ptr<Thread> &script) {
        m_scripts.push_back(script);
    }

    void LuaEngine::createMathTypes(sol::state &L) {
        using namespace glm;

        auto vec2_type = L.new_usertype<vec2>("vec2",
                                              sol::constructors<
                                                      glm::vec2(),
                                                      glm::vec2(float),
                                                      vec2(float, float)
                                              >()
        );

        vec2_type["x"] = &vec2::x;
        vec2_type["y"] = &vec2::y;

        vec2_type[sol::meta_function::to_string] = static_cast<std::string(*)(const vec2 &)>(&glm::to_string);

        vec2_type[sol::meta_function::addition] = sol::overload(
                static_cast<vec2(*)(const vec2 &, const vec2 &)>(&glm::operator+),
                static_cast<vec2(*)(float, const vec2 &)>(&glm::operator+),
                static_cast<vec2(*)(const vec2 &, float)>(&glm::operator+));

        vec2_type[sol::meta_function::subtraction] = sol::overload(
                static_cast<vec2(*)(const vec2 &, const vec2 &)>(&glm::operator-),
                static_cast<vec2(*)(float, const vec2 &)>(&glm::operator-),
                static_cast<vec2(*)(const vec2 &, float)>(&glm::operator-));

        vec2_type[sol::meta_function::multiplication] = sol::overload(
                static_cast<vec2(*)(const vec2 &, const vec2 &)>(&glm::operator*),
                static_cast<vec2(*)(float, const vec2 &)>(&glm::operator*),
                static_cast<vec2(*)(const vec2 &, float)>(&glm::operator*));

        vec2_type[sol::meta_function::division] = sol::overload(
                static_cast<vec2(*)(const vec2 &, const vec2 &)>(&glm::operator/),
                static_cast<vec2(*)(float, const vec2 &)>(&glm::operator/),
                static_cast<vec2(*)(const vec2 &, float)>(&glm::operator/));

        vec2_type[sol::meta_function::unary_minus] = [](const glm::vec2& v) { return -v; };

        vec2_type["normalize"] = normalize<2, float, glm::defaultp>;

        UNUSED(vec2_type);
    }

    void LuaEngine::createBodyType(sol::state &L)
    {
        // Use DataBody and not b2Body. Why?
        // Because DataBody is copied so can be safely used even if the body is deleted
        // However you still need to check body.valid before using it or it will throw NPE.

        auto type = L.new_usertype<DataBody>("DataBody");

        type["velocity"] = sol::property(&DataBody::getVelocity, &DataBody::setVelocityWithAngle);
        type["position"] = sol::property(&DataBody::getPosition, &DataBody::setPosition);
        type["angle"] = sol::property(&DataBody::getAngle, &DataBody::setAngle);
        type["valid"] = sol::property(&DataBody::isValid);
    }

    void LuaEngine::createRectType(sol::state &L)
    {
        auto type = L.new_usertype<Rect>("Rect");

        type["origin"] = &Rect::origin;
        type["size"] = &Rect::size;
        type["top"] = sol::property(&Rect::top);
        type["bottom"] = sol::property(&Rect::bottom);
        type["left"] = sol::property(&Rect::left);
        type["right"] = sol::property(&Rect::right);
        type["center"] = sol::property(&Rect::getCenter, &Rect::setOriginFromCenter);
    }
}