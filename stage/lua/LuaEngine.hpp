#pragma once

#include "stage/lua/LuaAPI.hpp"
#include "stage/lua/Thread.hpp"
#include "stage/StageSceneNode.hpp"
#include "Fwd.hpp"

namespace SpaceNinja::script
{
    /// @brief Manage the Stage scripts.
    /// @details Each step will run one step of the engine's script.
    class LuaEngine : public StageSceneNode
    {
    public:
        /// @brief Run the associated script to the Stage.
        explicit LuaEngine(Stage& stage);

        void onStep();

        /// @brief Register math types (glm library).
        static void createMathTypes(sol::state& L);

        /// @brief Register b2Body type.
        static void createBodyType(sol::state& L);

        /// @brief Register Rect type.
        static void createRectType(sol::state& L);

        /// @brief Add a script to the stage.
        void addScript(std::shared_ptr<Thread>& script);

    private:
        void startMainFunction();

        sol::state L;
        LuaAPI m_api;

        /// @brief Scripts currently run in the stage
        /// @remarks std::list to support insertion while iterating.
        std::list<std::shared_ptr<Thread>> m_scripts;
    };
}

