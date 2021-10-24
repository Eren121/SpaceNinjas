#include "gameplay/Stage.hpp"
#include "gameplay/Body.hpp"
#include "gameplay/VictoryMenu.hpp"
#include "gameplay/player/PlayerControl.hpp"
#include "gameplay/player/PlayerShoot.hpp"
#include "ui/VerticalListMenu.hpp"
#include "Game.hpp"
#include "wrappers/lua/LuaException.hpp"
#include "utility/Exception.hpp"
#include "utility/Str.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

Stage::Stage(SpaceNinja::Game &game, int id)
    : m_game(game),
      m_id(id),
      m_player(nullptr),
      m_world(std::make_shared<StageWorld>(game)),
      m_victory(Victory::Running)
{
    spawnPlayer();
    spawnVoid();
    
    addChild(m_world);

    addChild(std::make_unique<SpaceNinja::PlayerControl>(*this));
    addChild(std::make_unique<SpaceNinja::PlayerShoot>(*this));
    addChild(std::make_shared<SpaceNinja::script::LuaEngine>(*this));
}

Stage::~Stage() = default;

void Stage::spawnPlayer()
{
    Rect coords; // pos and size of the player
    coords.size = {2.0f, 2.0f};
    coords.setCenter(clipToWorldSpace({0.0f, 0.0f}));

    auto& player = m_world->createBoxBody(coords, b2_dynamicBody, 5.0f);
    m_player = &player;

    Body *userData = new Body(Body::Player, player, coords.radius());
    player.GetUserData() = userData;

    userData->setTexture(&m_game.textures("player.png"));

    // End the stage when the player is killed
    m_world->onDestroy.connect([this](b2Body& body) {
        if(&body == m_player)
        {
            m_player = nullptr;
            m_victory = Victory::Loss;
        }
    });
}

void Stage::spawnVoid()
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
        body.GetUserData() = userData;
        userData->setTexture(&m_game.textures("container.jpg"));
    }
}

bool Stage::updateNode()
{
    bool ret = true;

    if(m_victory != Victory::Running)
    {
        // The player has win or lost
        // In either way we stop the Stage
        ret = false;
        
        getLogger().debug("Level finished by the player. Result: {}", to_string(m_victory));
        
        if(m_victory == Victory::Win)
        {
            // Unlock the next level (if not already unlocked)
            const int nextID{m_id + 1};
            
            if(m_game.getSave().getLastLevelUnlocked() < nextID)
            {
                m_game.getSave().setLastLevelUnlocked(nextID);
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

    updateUI();

    return ret;
}

void Stage::updateUI()
{
    if(m_game.getControls().menuBack.isJustPressed())
    {
        // Create the menu
        auto menu = std::make_shared<
            SpaceNinja::ui::VerticalListMenu>(m_game);

        // Add the option to resume
        menu->addOption("Resume", [this](SceneNode& menu) {

            // Close the menu
            menu.removeNode();

            // Resume the simulation
            m_world->getClock().setPaused(false);
        });

        // Add the option to bring back to menu stage selection
        menu->addOption("Abandon", [this](SceneNode& menu) {

            // Close the menu
            menu.removeNode();

            // Stops the simulation
            this->removeNode();
        });

        // Show the menu
        addChild(menu);

        // Pause the simulation
        m_world->getClock().setPaused(true);
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

SpaceNinja::Game& Stage::getGame()
{
    return m_game;
}

const SpaceNinja::Game& Stage::getGame() const
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

glm::vec2 Stage::clipToWorldSpace(const glm::vec2 clipPos) const
{
    // Reverse MVP projection
    // Model Matrix is identity because we need point (0, 0)
    return glm::inverse(m_game.getViewMatrix())
           * glm::inverse(m_game.getProjectionMatrix())
           * glm::vec4(clipPos, 0.0f, 1.0f);
}

glm::vec2 Stage::worldToClipSpace(glm::vec2 worldPos) const
{
    return m_game.getProjectionMatrix() * m_game.getViewMatrix() * glm::vec4(worldPos, 0.0f, 1.0f);
}

void Stage::debugNode()
{
    const ImGuiInputTextFlags readOnly = ImGuiInputTextFlags_ReadOnly;

    if(ImGui::CollapsingHeader(getLogger().name().c_str()))
    {
        ImGui::InputInt("Stage ID", &m_id, 1, 0, readOnly);

        if (hasPlayer())
        {
            b2Body& player = getPlayer();
            glm::vec2 pos = b2::toGLM(player.GetPosition());

            if (ImGui::InputFloat2("Player position", &pos.x))
            {
                b2::setPosition(player, pos);
            }
        }
    }
}
