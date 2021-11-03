#pragma once

#include "wrappers/box2d/World.hpp"
#include <snk/logging.hpp>
#include "process/Process.hpp"
#include "Fwd.hpp"
#include <sol/sol.hpp>


namespace SpaceNinja::script
{
    /// @brief An instance of this class is in the Lua code, named 'api'.
    /// @details The functions of this class are bound to Lua.
    class LuaAPI
    {
        LOGGABLE("LuaAPI")

    public:
        explicit LuaAPI(Stage& stage);

        /// @brief Register the LuaAPI class as a new type.
        /// @remarks
        /// Also register the process "Wait" to wait for physics time. usage: coroutine.yield(Wait(millis)).
        /// However, we register "Wait" as a function that returns a std::shared_ptr<Wait> because, 1) there is
        /// no appropriate constructor and 2) we need a pointer.
        /// @remarks
        /// However, we can't use polymorphism in return type because sol2 will use static type information for
        /// type checking, and different templates types for smart pointers are unrelated in inheritance. The only allowed
        /// returned type for yields should be std::shared_ptr<Process> and not std::shared_ptr<SubProcessType>.
        static void createNewType(sol::state& L);

        /// @brief Spawn an enemy in the world.
        /// @return The spawned body.
        DataBody spawnEnemy(const glm::vec2& pos);

        /// @brief Starts an asynchronous task.
        /// @details The task will pushSceneStage immediately after invoking run(), after the next sleep of the script but
        /// on the same frame.
        /// The call of run() returns immediately.
        /// @param function The function to run as an asynchronous coroutine. The function should not take any argument
        /// and not return any value.
        void run(const sol::function& function);

        /// @brief Get the current iteration of the simulation
        long getIteration() const;

        /// @brief Get simulation time (in milliseconds).
        long getTime() const;

        /// @brief Stop the stage, the play won.
        void triggerWin();

        /// @brief Stop the stage, the player lost.
        void triggerLoss();

        /// @brief Set the speed of the game.
        /// @details
        ///     This does not change the internal step of the physics, that is always the same, but it can perform
        ///     more (if accelerated) or less steps each frame to simulated speed change but keeping right physics
        ///     simulation.
        float getTimeScale() const;
        void setTimeScale(float timeScale);

        /// @brief Get the playable area (viewport and move) of the player, in world unit.
        Rect getBounds() const;

        /// @remarks returns zero if there is no player
        glm::vec2 getPlayerPos() const;

        DataBody getPlayer();

        /// @return A process that wait the desired amount and then finishes.
        std::shared_ptr<Snow::exe::Process> wait(int millis) const;

        /// @brief Return the count of remaining living enemies in the world.
        int getEnemyCount() const;

        /// @returns A process that send a story message to the player in a box on the screen.
        /// @remarks
        ///     The task stops only when the player closes the dialog, either by clicking on a skip button o
        ///     if the maximum duration is elapsed.
        /// @param msg The message to print.
        /// @param duration The maximum duration of the message (in milliseconds).
        std::shared_ptr<Snow::exe::Process> sendMessage(const std::string& msg, int duration);

    private:

        Stage& m_stage;

        /// @brief Counter of remaining living enemies.
        int m_enemyCount{0};
    };
}

