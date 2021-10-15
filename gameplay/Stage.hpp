#pragma once

#include "Victory.hpp"
#include "PlayerControl.hpp"
#include "PlayerShoot.hpp"
#include "CollisionManager.hpp"
#include "StageWorld.hpp"
#include "UIStage.hpp"
#include "gameplay/lua/LuaEngine.hpp"
#include "process/ProcessPool.hpp"
#include <utility/logging.hpp>

class Game;

class Stage : public SceneNode, public Process, public Loggable<"Stage">
{
public:
    explicit Stage(Game& game, int id);
    ~Stage() override;
    
protected:
    /// @remarks Wraps the Process update()
    bool updateNode() override;
    
    void update() override;
    
private:
    /// @brief Synchronize IRL and the simulation
    /// @details That is run the simulation until the clock are synchronized.
    void syncWorld();
    
public:
    b2::World& getWorld();
    const b2::World& getWorld() const;
    
    Game& getGame();
    const Game& getGame() const;
    
    /// @brief When getting the player, you must ensure the player exists (not dead).
    bool hasPlayer() const;
    
protected:
    void debugNode() override;

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
    
private:
    void initPlayer();
    void initVoid();

    Game& m_game;
    int m_id;

    b2Body *m_player;
    bool m_firstUpdate;
    
    std::shared_ptr<StageWorld> m_world;

    /// @brief Simulation starting time, to synchronize the simulation with IRL.
    Time m_start;
    
    std::shared_ptr<LuaEngine> m_luaEngine;
    std::shared_ptr<PlayerControl> m_playerControl;
    std::shared_ptr<PlayerShoot> m_playerShoot;
    std::shared_ptr<UIStage> m_uiRenderer;
    
    Victory m_victory;
    
public:
    ProcessPool eachStep; ///< Processes to run each step
    
private:
    Logger::pointer m_logger;
};

