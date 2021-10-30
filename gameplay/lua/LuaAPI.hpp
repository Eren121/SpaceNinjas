#pragma once

#include "wrappers/box2d/World.hpp"
#include "utility/logging.hpp"
#include "process/Process.hpp"
#include "Fwd.hpp"
#include <sol/sol.hpp>


namespace SpaceNinja::script
{
    /// @brief An instance of this class is in the Lua code, named 'api'.
    /// @details The functions of this class are bound to Lua.
    class LuaAPI : public Loggable<"LuaAPI">
    {
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

        /// @brief Spawn an ennemy
        /// @param x, y The position of the ennemy in clip space
        b2Body& spawnEnnemy(float clipX, float clipY, float velX, float velY);

        /// @brief Starts an asynchronous task.
        /// @details The task will pushSceneStage immediately after invoking run(), after the next sleep of the script but
        /// on the same frame.
        /// The call of run() returns immediately.
        /// @param function The function to run as an asynchronous coroutine. The function should not take any argument
        /// and not return any value.
        void run(const sol::function& function);

        /// @brief Get the current iteration of the simulation
        int getIteration() const;

        // @brief Get simulation time (in milliseconds)
        long getTime() const;

        void win();
        void defeat();

        float getTimeScale() const;
        void setTimeScale(float timeScale);

        /// @remarks returns zero if there is no player
        glm::vec2 getPlayerPos() const;

        std::shared_ptr<Snow::exe::Process> wait(int millis) const;

        int ennemyCount() const;

        /// @returns A task that send a story message to the player in a box on the screen.
        /// @remarks
        ///     The task returns only when the player closes the dialog, either by clicking on a skip button o
        ///     if time is elapsed.
        /// @param msg The message to print.
        /// @param millis The maximum duration of the message.
        std::shared_ptr<Snow::exe::Process> sendMessage(const std::string& msg, int millis);

    private:

        Stage& m_stage;

        /// @brief Count of remaining living ennemies.
        int m_ennemyCount{0};
    };
}

