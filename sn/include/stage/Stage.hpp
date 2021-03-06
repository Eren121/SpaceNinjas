#pragma once

#include "DataBody.hpp"
#include "Victory.hpp"
#include "CollisionManager.hpp"
#include "StageWorld.hpp"
#include "stage/lua/LuaEngine.hpp"
#include "Fwd.hpp"
#include <process/ProcessPool.hpp>
#include <logging.hpp>

namespace sn
{
    class Stage : public SceneNode
    {
        LOGGABLE("Stage")

    public:
        explicit Stage(Game &game, int id);

        ~Stage() override;

    protected:
        bool updateNode() override;

    public:
        StageWorld &getWorld();
        const StageWorld &getWorld() const;

        Game &getGame();
        const Game &getGame() const;

        script::LuaEngine &getScriptEngine() { return *m_scriptEngine; }

        /// @brief When getting the player, you must ensure the player exists (not dead).
        bool hasPlayer() const;

    protected:
        void debugNode() override;

        /// @brief Check UI inputs like to open main menu, etc.
        void openMenuIfAsked();

    public:
        DataBody getPlayer() const;

        void stopStage(Victory victory);

        Victory getVictory() const;

        int getID() const;

        /// @brief Map a position in clip space to world space
        /// @details
        /// For example, clipPos = (0, 0) will map to world point that is on the center of the window
        glm::vec2 clipToWorldSpace(glm::vec2 clipPos) const;
        glm::vec2 worldToClipSpace(glm::vec2 worldPos) const;

        /// @brief Convert a point in normalized coordinates (-1 to 1) to world coordinates.
        /// @details
        ///     It never change during a game, even if the player view changes.
        glm::vec2 mapPointNormToWorld(const glm::vec2& normCoords) const;

        /// @brief Build the view matrix from the viewport bounds.
        glm::mat4 getViewMatrix() const;

        const Rect& getBounds() const { return m_bounds; }

    private:
        void spawnPlayer();

        /// @details
        ///     The player can't go outside the screen, the limits are box around it to ensure that.
        void spawnPlayerLimits();

        /// @details
        ///     The universe defines a 'hard limit', when any body of any type go outside this zone, it is destroyed.
        ///     It is like garbage collection to avoid having bodies count increase with useless bodies at km of the main zone.
        void spawnUniverse();

        Game &m_game;
        int m_id;

        DataBody m_player;

        std::shared_ptr<StageWorld> m_world;
        std::shared_ptr<script::LuaEngine> m_scriptEngine;

        Victory m_victory;

        // The Rect the player sees on the screen, in world coordinates
        Rect m_bounds;

        // The amount of zoom. If more than 1, the viewport is smaller and objects are biggers.
        // Set for debugging purposes and maybe effects in game.
        // Do not affects bounds, but only the view matrix.
        float m_zoom{1.0f};

        bool m_inMenu{false};
    };
}
