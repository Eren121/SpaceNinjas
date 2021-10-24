#pragma once

#include "gameplay/Victory.hpp"
#include "gameplay/CollisionManager.hpp"
#include "gameplay/StageWorld.hpp"
#include "gameplay/lua/LuaEngine.hpp"
#include "process/ProcessPool.hpp"
#include "Fwd.hpp"
#include "utility/logging.hpp"

class Stage : public SceneNode, public Loggable<"Stage">
{
public:
    explicit Stage(SpaceNinja::Game& game, int id);
    ~Stage() override;
    
protected:
    bool updateNode() override;

public:
    b2::World& getWorld();
    const b2::World& getWorld() const;
    
    SpaceNinja::Game& getGame();
    const SpaceNinja::Game& getGame() const;
    
    /// @brief When getting the player, you must ensure the player exists (not dead).
    bool hasPlayer() const;
    
protected:
    void debugNode() override;

    /// @brief Check UI inputs like to open main menu, etc.
    void updateUI();

public:
    b2Body& getPlayer();
    const b2Body& getPlayer() const;

    void stopStage(Victory victory);
    Victory getVictory() const;

    int getID() const;

    /// @brief Map a position in clip space to world space
    /// @details
    /// For example, clipPos = (0, 0) will map to world point that is on the center of the window
    glm::vec2 clipToWorldSpace(glm::vec2 clipPos) const;
    glm::vec2 worldToClipSpace(glm::vec2 worldPos) const;

private:
    void spawnPlayer();
    void spawnVoid();

    SpaceNinja::Game& m_game;
    int m_id;

    b2Body *m_player;
    
    std::shared_ptr<StageWorld> m_world;
    
    Victory m_victory;
};

