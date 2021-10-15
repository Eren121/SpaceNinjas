#include "LuaAPI.hpp"
#include "LuaStageCoroutine.hpp"
#include "Game.hpp"
#include "gameplay/Stage.hpp"
#include "gameplay/Body.hpp"
#include "process/Wait.hpp"
#include <utility/math.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

LuaAPI::LuaAPI(Stage& stage)
    : m_stage(stage)
{
    getLogger().set_level(spdlog::level::debug);
    
    auto& world = m_stage.getWorld();
    world.onDestroy.connect([this](b2Body& b2body) {
        Body *body = b2::getUserData<Body>(b2body);
        
        if(body->type == Body::Ennemy)
        {
            m_ennemyCount--;
            getLogger().debug("Ennemy destroyed (remaining: {})", m_ennemyCount);
        }
    });
}


void LuaAPI::createNewType(sol::state& L)
{
    auto type = L.new_usertype<LuaAPI>("LuaAPI");
    
    // Register methods
    // Do not add constructors, to forbid to construct it (anyway the constructor is not accessible)

#define LINK_API(method) type[#method] = &LuaAPI::method

    LINK_API(spawnEnnemy);
    LINK_API(win);
    LINK_API(defeat);
    LINK_API(run);
    LINK_API(ennemyCount);
    LINK_API(wait);
}

b2Body& LuaAPI::spawnEnnemy(float x, float y, float velX, float velY)
{
    getLogger().debug("Calling spawnEnnemy({}, {}, {}, {})", x, y, velX, velY);
    
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
    b2::setUserData(body, user);

    return body;
}

void LuaAPI::win()
{
    getLogger().debug("Calling win()");
    
    m_stage.stopStage(Victory::Win);
}

void LuaAPI::defeat()
{
    getLogger().debug("Calling defeat()");
    
    m_stage.stopStage(Victory::Loss);
}

void LuaAPI::run(const sol::function& function)
{
    getLogger().debug("Calling run(<function>)");
    
    lua_State *L = function.lua_state();
    
    // Push the function on top of the stack
    // Note: registry_index() is NOT the index on the stack, but the index on the registry
    // Sol2 use the registry to avoid garbage collection
    lua_pushinteger(L, function.registry_index());
    lua_gettable(L, LUA_REGISTRYINDEX);
    
    // If the lua function has no arguments, the argument will just be ignored.
    lua_pushinteger(L, m_stage.getID());
    
    auto coro = std::make_shared<LuaStageCoroutine>(L, 1);
    m_stage.eachStep.add(coro);
}

int LuaAPI::ennemyCount() const
{
    getLogger().debug("Calling ennemyCount() (returns: {})", m_ennemyCount);
    
    return m_ennemyCount;
}

std::shared_ptr<Process> LuaAPI::wait(int millis) const
{
    getLogger().debug("Calling wait({})", millis);
    
    auto source = [this]() {
        return m_stage.getWorld().getTime();
    };
    
    auto process = std::make_shared<Wait>(Time::milliseconds(millis), source);
    return process;
}
