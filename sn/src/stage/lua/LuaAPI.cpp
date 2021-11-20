#include "Game.hpp"
#include "stage/lua/LuaAPI.hpp"
#include "stage/lua/Thread.hpp"
#include "stage/Stage.hpp"
#include "stage/physics/BodyBuilder.hpp"
#include "ui/StoryMessage.hpp"
#include "gameplay/components/Hostile.hpp"
#include <process/Wait.hpp>
#include <process/Process.hpp>
#include <math.hpp>
#include <unused.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace sn::script
{
    LuaAPI::LuaAPI(Stage& stage)
        : m_stage(stage)
    {
        getLogger().set_level(spdlog::level::debug);
    }


    void LuaAPI::createNewType(sol::state& L)
    {

        auto api = L.new_usertype<LuaAPI>("LuaAPI",
            "iteration", sol::property(&LuaAPI::getIteration),
            "time", sol::property(&LuaAPI::getTime));

        // Register methods
        // Do not add constructors, to forbid to construct it (anyway the constructor is not accessible)

        api["spawnEnemy"] = &LuaAPI::spawnEnemy;
        api["win"] = &LuaAPI::triggerWin;
        api["defeat"] = &LuaAPI::triggerLoss;
        api["run"] = &LuaAPI::run;
        api["enemyCount"] = &LuaAPI::getEnemyCount;
        api["wait"] = &LuaAPI::wait;
        api["message"] = &LuaAPI::sendMessage;
        api["playerPos"] = &LuaAPI::getPlayerPos;

        api["timeScale"] = sol::property(&LuaAPI::getTimeScale, &LuaAPI::setTimeScale);
        api["bounds"] = sol::property(&LuaAPI::getBounds);
        api["player"] = sol::property(&LuaAPI::getPlayer);
    }

    DataBody LuaAPI::spawnEnemy(const glm::vec2& pos)
    {
        getLogger().debug("Calling spawnEnemy(pos={})", pos);
        return createEnemyBody(m_stage.getWorld(), pos);
    }

    void LuaAPI::triggerWin()
    {
        getLogger().trace("Calling win()");

        m_stage.stopStage(Victory::Win);
    }

    void LuaAPI::triggerLoss()
    {
        getLogger().trace("Calling triggerLoss()");

        m_stage.stopStage(Victory::Loss);
    }

    void LuaAPI::run(const sol::function& function)
    {
        getLogger().trace("Calling run(<function>)");

        lua_State *L = function.lua_state();

        // Push the function on top of the stack
        // Note: registry_index() is NOT the index on the stack, but the index on the registry
        // Sol2 use the registry to avoid garbage collection
        lua_pushinteger(L, function.registry_index());
        lua_gettable(L, LUA_REGISTRYINDEX);

        // If the lua function has no arguments, the argument will just be ignored.
        lua_pushinteger(L, m_stage.getID());

        auto coro = std::make_shared<Thread>(L, 1);

        m_stage.getScriptEngine().addScript(coro);
    }

    int LuaAPI::getEnemyCount() const
    {

        // Single type view know the exact amount of entities
        // For "obvious reasons"...
        // Not obvious to me but we will believe it and don't ask :D
        // https://github.com/skypjack/entt/issues/567

        auto view{m_stage.getWorld().getRegistry().view<Hostile>()};
        const auto enemyCount{static_cast<int>(view.size())};

        getLogger().trace("Calling getEnemyCount() (returns: {})", enemyCount);

        return enemyCount;
    }

    std::shared_ptr<Snow::exe::Process> LuaAPI::wait(int millis) const
    {
        getLogger().trace("Calling wait({})", millis);

        auto source = [this]() {
            return m_stage.getWorld().getTime();
        };

        auto process = std::make_shared<Snow::exe::Wait>(Time::milliseconds(millis), source);
        return process;
    }

    long LuaAPI::getIteration() const
    {
        return m_stage.getWorld().getIteration();
    }

    long LuaAPI::getTime() const
    {
        return m_stage.getWorld().getTime().asMilliseconds();
    }

    glm::vec2 LuaAPI::getPlayerPos() const
    {
        if(m_stage.hasPlayer())
        {
            glm::vec2 worldPos{m_stage.getPlayer().getPosition()};
            return worldPos;
        }
        else
        {
            return {0, 0};
        }
    }

    std::shared_ptr<Snow::exe::Process> LuaAPI::sendMessage(const std::string& msg, int duration)
    {
        auto task = std::make_shared<sn::ui::StoryMessage>(m_stage);
        task->setDuration(Time::milliseconds(duration));
        task->setMessage(msg);

        return task;
    }

    float LuaAPI::getTimeScale() const
    {
        return m_stage.getWorld().getClock().getSpeed();
    }

    void LuaAPI::setTimeScale(float timeScale)
    {
        m_stage.getWorld().getClock().setSpeed(timeScale);
    }

    Rect LuaAPI::getBounds() const
    {
        return m_stage.getBounds();
    }

    DataBody LuaAPI::getPlayer()
    {
        return m_stage.getPlayer();
    }
}