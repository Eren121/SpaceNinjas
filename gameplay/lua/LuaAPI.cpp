#include "LuaAPI.hpp"
#include "ProcessCoroutine.hpp"
#include "Game.hpp"
#include "gameplay/Stage.hpp"
#include "gameplay/Body.hpp"
#include "process/Wait.hpp"
#include "process/Process.hpp"
#include "utility/math.hpp"
#include "utility/macro/unused.hpp"
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

SpaceNinja::script::LuaAPI::LuaAPI(Stage& stage)
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


void SpaceNinja::script::LuaAPI::createNewType(sol::state& L)
{

    auto api = L.new_usertype<LuaAPI>("LuaAPI",
        "iteration", sol::property(&LuaAPI::getIteration),
        "time", sol::property(&LuaAPI::getTime));
    
    // Register methods
    // Do not add constructors, to forbid to construct it (anyway the constructor is not accessible)

#define LINK_API(method) api[#method] = &LuaAPI::method

    LINK_API(spawnEnnemy);
    LINK_API(win);
    LINK_API(defeat);
    //LINK_API(run);
    LINK_API(ennemyCount);
    LINK_API(wait);

    //api["player_pos"] = &LuaAPI::getPlayerPos;

    UNUSED(api);

#undef LINK_API
}

b2Body& SpaceNinja::script::LuaAPI::spawnEnnemy(float x, float y, float velX, float velY)
{
    getLogger().trace("Calling spawnEnnemy({}, {}, {}, {})", x, y, velX, velY);
    
    m_ennemyCount++;

    glm::vec2 velocity = {velX, velY};

    Rect box;
    box.setCenter(m_stage.clipToWorldSpace({x, y}));
    box.size = {2.0f, 2.0f};
    
    auto& world = m_stage.getWorld();
    auto& body = world.createBoxBody(box);
    body.SetLinearVelocity(b2::fromGLM(velocity));
    b2::setAngle(body, math::vec2angle(velocity));

    Body *user = new Body(Body::Ennemy, body, box.size / 2.0f);
    user->setTexture(&m_stage.getGame().textures("ennemy.png"));
    body.GetUserData() = user;

    return body;
}

void SpaceNinja::script::LuaAPI::win()
{
    getLogger().trace("Calling win()");
    
    m_stage.stopStage(Victory::Win);
}

void SpaceNinja::script::LuaAPI::defeat()
{
    getLogger().trace("Calling defeat()");
    
    m_stage.stopStage(Victory::Loss);
}

void SpaceNinja::script::LuaAPI::run(const sol::function& function)
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
    
    auto coro = std::make_shared<ProcessCoroutine>(L, 1);

    //TODO EACH STEP
    //m_stage.eachStep.add(coro);
}

int SpaceNinja::script::LuaAPI::ennemyCount() const
{
    getLogger().trace("Calling ennemyCount() (returns: {})", m_ennemyCount);
    
    return m_ennemyCount;
}

std::shared_ptr<Snow::exe::Process> SpaceNinja::script::LuaAPI::wait(int millis) const
{
    getLogger().trace("Calling wait({})", millis);
    
    auto source = [this]() {
        return m_stage.getWorld().getTime();
    };
    
    auto process = std::make_shared<Snow::exe::Wait>(Time::milliseconds(millis), source);
    return process;
}

int SpaceNinja::script::LuaAPI::getIteration() const
{
    return m_stage.getWorld().getIteration();
}

long SpaceNinja::script::LuaAPI::getTime() const
{
    return m_stage.getWorld().getTime().asMilliseconds();
}

glm::vec2 SpaceNinja::script::LuaAPI::getPlayerPos() const
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
