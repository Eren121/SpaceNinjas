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

            if(body->type == Body::Ennemy)
            {
                m_ennemyCount--;
                getLogger().trace("Ennemy destroyed (remaining: {})", m_ennemyCount);
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

        api["spawnEnnemy"] = &LuaAPI::spawnEnnemy;
        api["win"] = &LuaAPI::win;
        api["defeat"] = &LuaAPI::defeat;
        api["run"] = &LuaAPI::run;
        api["ennemyCount"] = &LuaAPI::ennemyCount;
        api["wait"] = &LuaAPI::wait;
        api["message"] = &LuaAPI::sendMessage;
        api["playerPos"] = &LuaAPI::getPlayerPos;

        api["timeScale"] = sol::property(&LuaAPI::getTimeScale, &LuaAPI::setTimeScale);

        UNUSED(api);

    #undef LINK_API
    }

    b2Body& LuaAPI::spawnEnnemy(float x, float y, float velX, float velY)
    {
        getLogger().trace("Calling spawnEnnemy({}, {}, {}, {})", x, y, velX, velY);

        m_ennemyCount++;

        glm::vec2 velocity = {velX, velY};

        const Texture *playerTexture = &m_stage.getGame().textures("ennemy.png");
        const glm::vec2 playerTextureSize = playerTexture->getSize();
        const float ratio = playerTextureSize.y / playerTextureSize.x;
        const float fixedSizeX = 2.0f;

        Rect box;
        box.setCenter(m_stage.mapPointNormToWorld({x, y}));
        box.size = {fixedSizeX, fixedSizeX * ratio};

        auto& world = m_stage.getWorld();
        auto& body = world.createBoxBody(box);
        body.SetLinearVelocity(b2::fromGLM(velocity));
        b2::setAngle(body, math::vec2angle(velocity));

        Body *user = new Body(Body::Ennemy, body, box.size / 2.0f);
        user->setTexture(playerTexture);
        body.GetUserData() = user;

        return body;
    }

    void LuaAPI::win()
    {
        getLogger().trace("Calling win()");

        m_stage.stopStage(Victory::Win);
    }

    void LuaAPI::defeat()
    {
        getLogger().trace("Calling defeat()");

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

    int LuaAPI::ennemyCount() const
    {
        getLogger().trace("Calling ennemyCount() (returns: {})", m_ennemyCount);

        return m_ennemyCount;
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

    int LuaAPI::getIteration() const
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
            return m_stage.worldToClipSpace(worldPos);
        }
        else
        {
            return {0, 0};
        }
    }

    std::shared_ptr<Snow::exe::Process> LuaAPI::sendMessage(const std::string& msg, int millis)
    {
        auto task = std::make_shared<SpaceNinja::ui::StoryMessage>(m_stage);
        task->setDuration(Time::milliseconds(millis));
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
}