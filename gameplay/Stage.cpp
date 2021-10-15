#include "Stage.hpp"
#include "Body.hpp"
#include "Game.hpp"
#include "VictoryMenu.hpp"
#include "utility/Exception.hpp"
#include "utility/Str.hpp"
#include "utility/macro/unused.hpp"
#include "wrappers/lua/LuaException.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

Stage::Stage(Game &game, int id)
    : m_game(game),
      m_id(id),
      m_player(nullptr),
      m_firstUpdate(false),
      m_world(std::make_shared<StageWorld>(game)),
      m_luaEngine(std::make_shared<LuaEngine>(*this)),
      m_playerControl(std::make_unique<PlayerControl>(*this)),
      m_uiRenderer(std::make_shared<UIStage>(game, *this)),
      m_victory(Victory::Running),
      m_logger{Logger::getOrCreate("Stage")}
{
    initPlayer();
    initVoid();
    
    addChild(m_world);
    addChild(m_uiRenderer);
    
    eachStep.add(m_luaEngine);
}

void Stage::initPlayer()
{
    Rect coords; // pos and size of the player
    coords.size = {2.0f, 2.0f};
    coords.setCenter(clipToWorldSpace({0.0f, 0.0f}));

    auto& player = m_world->createBoxBody(coords, b2_dynamicBody, 5.0f);
    m_player = &player;

    Body *userData = new Body(Body::Player, player, coords.radius());
    b2::setUserData(player, userData);

    userData->setTexture(&m_game.textures("player.png"));

    addChild(m_playerControl);

    m_playerShoot = std::make_shared<PlayerShoot>(*this);
    addChild(m_playerShoot);

    // End the stage when the player is killed
    m_world->onDestroy.connect([this](b2Body& body) {
        if(&body == m_player)
        {
            m_player = nullptr;
            m_victory = Victory::Loss;
        }
    });
}

void Stage::initVoid()
{
    // Rect radius, further bodies will instantly be destroyed
    float worldRadius = 50.0f;

    // Should be enough thick to never let an unit pass
    float voidThickness = 10.0f;

    Rect left({voidThickness, worldRadius * 2.0f});
    left.setCenter({-worldRadius -voidThickness / 2.0f, 0});

    Rect right(left.size);
    right.setCenter(-left.center()); // Build others borders based on the left one

    Rect up({right.size.y, right.size.x});
    up.setCenter({0, right.center().x});

    Rect down(up.size);
    down.setCenter(-up.center());

    for(const Rect& rect : {left, right, up, down})
    {
        b2Body& body = m_world->createBoxBody(rect, b2_staticBody);

        Body *userData = new Body(Body::Void, body, rect.size / 2.0f);
        b2::setUserData(body, userData);
        userData->setTexture(&m_game.textures("container.jpg"));
    }
}

bool Stage::updateNode()
{
    return operator()();
}

void Stage::update()
{
    if(m_firstUpdate)
    {
        m_firstUpdate = false;
        m_start = Time::now();
    }

    syncWorld();

    if(m_victory != Victory::Running)
    {
        // The player has win or lost
        // In either way we stop the process
        stop();
        
        getLogger().debug("Level finished by the player. Result: {}", to_string(m_victory));
        
        if(m_victory == Victory::Win)
        {
            // Unlock the next level (if not already unlocked)
            const int nextID{m_id + 1};
            
            if(m_game.getSave().lastLevelUnlocked < nextID)
            {
                m_game.getSave().lastLevelUnlocked = nextID;
                m_game.writeSave();
                
                getLogger().debug("Level {} unlocked", nextID);
            }
            else
            {
                getLogger().debug("Level {} already unlocked", nextID);
            }
        }
        
        
        //m_game.scene.addChild(std::make_shared<VictoryMenu>(m_game.getStagePtr()));
        ///TODO
    }
}

void Stage::syncWorld()
{
    // We don't need to be synchronized (not a mutliplayer game)
    // We do one step per frame
    // However, all the timers are sync with the world clock

    //while(m_world.getTime() + m_world.dt() <= (Time::now() - m_start))
    {
        m_world->step();
        eachStep();
    }
}

b2::World& Stage::getWorld()
{
    return *m_world;
}

const b2::World& Stage::getWorld() const
{
    return *m_world;
}

Game& Stage::getGame()
{
    return m_game;
}

const Game& Stage::getGame() const
{
    return m_game;
}

b2Body& Stage::getPlayer()
{
    return *m_player;
}

const b2Body& Stage::getPlayer() const
{
    return *m_player;
}

bool Stage::hasPlayer() const
{
    return m_player != nullptr;
}

void Stage::stopStage(Victory victory)
{
    m_victory = victory;
}

Victory Stage::getVictory() const
{
    return m_victory;
}

int Stage::getID() const
{
    return m_id;
}

Stage::~Stage() = default;

glm::vec2 Stage::clipToWorldSpace(const glm::vec2 clipPos) const
{
    // Reverse MVP projection
    // Model Matrix is identity because we need point (0, 0)
    return glm::inverse(m_game.getViewMatrix())
           * glm::inverse(m_game.getProjectionMatrix())
           * glm::vec4(clipPos, 0.0f, 1.0f);
}

void Stage::debugNode()
{
}
