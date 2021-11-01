#include "Game.hpp"
#include "gameplay/lua/LuaAPI.hpp"
#include "gameplay/lua/Thread.hpp"
#include "gameplay/Stage.hpp"
#include "gameplay/Body.hpp"
#include "ui/StoryMessage.hpp"
#include "process/Wait.hpp"
#include "process/Process.hpp"
#include "utility/math.hpp"
#include "utility/macro/unused.hpp"
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace SpaceNinja::script
{
    LuaAPI::LuaAPI(Stage& stage)
        : m_stage(stage)
    {
        getLogger().set_level(spdlog::level::debug);

        auto& world = m_stage.getWorld();
        world.onDestroy.connect([this](b2Body& b2body) {
            Body *body = b2body.GetUserData();

            if(body->type == Body::Enemy)
            {
                m_enemyCount--;
                getLogger().trace("Enemy destroyed (remaining: {})", m_enemyCount);
            }
        });
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
    }

    b2Body& LuaAPI::spawnEnemy(const glm::vec2& pos)
    {
        getLogger().debug("Calling spawnEnemy(pos={})", pos);

        m_enemyCount++;

        const Texture *playerTexture = &m_stage.getGame().textures("enemy.png");
        const glm::vec2 playerTextureSize = playerTexture->getSize();
        const float ratio = playerTextureSize.y / playerTextureSize.x;
        const float fixedSizeX = 2.0f;

        Rect box;
        box.size = {fixedSizeX, fixedSizeX * ratio};
        box.setOriginFromCenter(pos);

        auto& world = m_stage.getWorld();
        auto& body = world.createBoxBody(box);

        Body *user = new Body(Body::Enemy, body, box.size / 2.0f);
        user->setTexture(playerTexture);
        body.GetUserData() = user;

        return body;
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
        getLogger().trace("Calling getEnemyCount() (returns: {})", m_enemyCount);

        return m_enemyCount;
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
            glm::vec2 worldPos = b2::toGLM(m_stage.getPlayer().GetPosition());
            return worldPos;
        }
        else
        {
            return {0, 0};
        }
    }

    std::shared_ptr<Snow::exe::Process> LuaAPI::sendMessage(const std::string& msg, int duration)
    {
        auto task = std::make_shared<SpaceNinja::ui::StoryMessage>(m_stage);
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
}