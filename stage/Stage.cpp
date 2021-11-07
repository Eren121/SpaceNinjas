#include "DataFixture.hpp"
#include "Stage.hpp"
#include "VictoryMenu.hpp"
#include "player/PlayerControl.hpp"
#include "player/PlayerShoot.hpp"
#include "ui/VerticalListMenu.hpp"
#include "Game.hpp"
#include <snk/math.hpp>
#include <imgui.h>
#include "stage/physics/BodyBuilder.hpp"

namespace SpaceNinja
{
    Stage::Stage(Game &game, int id)
        : m_game(game),
          m_id(id),
          m_world(std::make_shared<StageWorld>(*this)),
          m_victory(Victory::Running)
    {
        // The bounds is constant, in world coordinates
        // So the game is designed for a specific resolution
        // Both width and height are constant, by we define the height relative to the width in 16:9
        const float viewportWidth{50.0f};

        m_bounds = Rect{{viewportWidth, viewportWidth / m_game.getAspectRatio()}};
        m_bounds.setOriginFromCenter({0.0f, 0.0f});

        spawnUniverse();
        spawnPlayerLimits();
        spawnPlayer();

        addChild(m_world);

        addChild(std::make_unique<PlayerControl>(*this));
        addChild(std::make_unique<PlayerShoot>(*this));
        addChild(m_scriptEngine = std::make_shared<script::LuaEngine>(*this));
    }

    Stage::~Stage() = default;

    void Stage::spawnPlayer()
    {
        m_player = createPlayerBody(*m_world, {0, 0});
    }

    void Stage::spawnPlayerLimits()
    {
        // The thickness of the limit boxes
        // Should be enough thick to never let an unit pass
        const float thickness{10.0f};

        const Rect bounds{m_bounds};
        const glm::vec2 center{bounds.getCenter()};

        Rect leftBox{{thickness, bounds.size.y}};
        leftBox.setOriginFromCenter(center - glm::vec2{bounds.size.x / 2.0f, 0.0f} - glm::vec2{thickness / 2.0f, 0.0f});

        Rect rightBox{{thickness, bounds.size.y}};
        rightBox.setOriginFromCenter(center + glm::vec2{bounds.size.x / 2.0f, 0.0f} + glm::vec2{thickness / 2.0f, 0.0f});

        Rect topBox{{bounds.size.x, thickness}};
        topBox.setOriginFromCenter(center + glm::vec2{0.0f, bounds.size.y / 2.0f} + glm::vec2{0.0f, thickness / 2.0f});

        Rect bottomBox{{bounds.size.x, thickness}};
        bottomBox.setOriginFromCenter(
                center - glm::vec2{0.0f, bounds.size.y / 2.0f} - glm::vec2{0.0f, thickness / 2.0f});

        for(const Rect& rect : {leftBox, rightBox, topBox, bottomBox})
        {
            b2BodyDef def;
            def.type = b2_staticBody;

            // Add the body in position (0, 0) but the fixture is on the right location so its ok

            DataBody& data = m_world->createBody(def);
            data.type = BodyType::PlayerLimits;
            addBoxToBody(data, rect);
        }
    }

    void Stage::spawnUniverse()
    {
        // Radius of the universe,
        // everything outside this circle will be killed
        // Get the minimum size to encompass the viewport and add a margin
        // Use a box instead of a circle as it is easier to handle different size in X/Y
        const glm::vec2 margin{20.0f};

        Rect bounds{{m_bounds.size + margin}};
        bounds.setOriginFromCenter(m_bounds.getCenter());

        b2BodyDef def;
        def.type = b2_staticBody;

        DataBody& body = m_world->createBody(def);

        addBoxToBody(body, bounds);
        body.type = BodyType::Universe;
        body.setSensor(true);
    }

    bool Stage::updateNode()
    {
        bool ret = true;

        // End the stage when the player is killed
        if(!m_player)
        {
            m_victory = Victory::Loss;
        }

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

        if(!m_inMenu) {
            openMenuIfAsked();
        }

        return ret;
    }

    void Stage::openMenuIfAsked()
    {
        if(m_game.getControls().menuBack.isJustPressed())
        {
            m_inMenu = true;
            
            // Create the menu
            auto menu = std::make_shared<
                ui::VerticalListMenu>(m_game);

            // Add the option to resume
            menu->addOption("Resume", [this](SceneNode& menu) {

                // Close the menu
                menu.removeNode();

                // Resume the simulation
                m_world->getClock().setPaused(false);

                m_inMenu = false;
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

    StageWorld& Stage::getWorld()
    {
        return *m_world;
    }

    const StageWorld& Stage::getWorld() const
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

    DataBody Stage::getPlayer() const
    {
        return m_player;
    }

    bool Stage::hasPlayer() const
    {
        return m_player.isValid();
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

    glm::mat4 Stage::getViewMatrix() const
    {
        // Zoom is opposite of scale so divide and not multiply
        Rect bounds{m_bounds.size / m_zoom};
        bounds.setOriginFromCenter(m_bounds.getCenter());

        return glm::ortho(bounds.left(), bounds.right(), bounds.bottom(), bounds.top());
    }

    glm::vec2 Stage::clipToWorldSpace(const glm::vec2 clipPos) const
    {
        // Reverse MVP projection
        // Model Matrix is identity because we need point (0, 0)
        return glm::inverse(getViewMatrix())
               * glm::vec4(clipPos, 0.0f, 1.0f);
    }

    glm::vec2 Stage::worldToClipSpace(glm::vec2 worldPos) const
    {
        return getViewMatrix() * glm::vec4(worldPos, 0.0f, 1.0f);
    }

    glm::vec2 Stage::mapPointNormToWorld(const glm::vec2& normCoords) const
    {
        const glm::vec2 unsignedNormCoords = math::normCoordsToUnsigned(normCoords);
        return m_bounds.origin + unsignedNormCoords * m_bounds.size;
    }

    void Stage::debugNode()
    {
        const ImGuiInputTextFlags readOnly = ImGuiInputTextFlags_ReadOnly;

        if(ImGui::CollapsingHeader(getLogger().name().c_str()))
        {
            ImGui::InputInt("Stage ID", &m_id, 1, 0, readOnly);

            if (hasPlayer())
            {
                b2Body& player = m_player.getBody();
                glm::vec2 pos = b2::toGLM(player.GetPosition());

                if (ImGui::InputFloat2("Player position", &pos.x))
                {
                    b2::setPosition(player, pos);
                }
            }

            // Debug textures as a Box (to see boundaries)

            static bool debugTextures = false;

            if(ImGui::Button("Reset")) { m_zoom = 1.0f; }
            ImGui::SameLine();
            ImGui::SliderFloat("Zoom level", &m_zoom, 0.1f, 3.0f);

            if(ImGui::Checkbox("Debug Textures", &debugTextures))
            {
                // Replace each body texture with a debug texture
                // Add a component to backup the texture to disable debug rendering later

                for(b2Body& body : *m_world)
                {
                    entt::const_handle handle = getData(body).handle;
                    if(!handle) continue;

                    for(b2Fixture& fixture : getData(body))
                    {
                        DataFixture& data = getData(fixture);
                        if(!data.sprite) continue;

                        if(debugTextures)
                        {
                            data.savedTexture = data.sprite->getTexture();
                            data.sprite->setTexture(&m_game.textures("@missing"));
                        }
                        else
                        {
                            data.sprite->setTexture(data.savedTexture);
                        }
                    }
                }
            }
        }
    }
}