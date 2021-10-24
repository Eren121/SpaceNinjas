#pragma once

#include "gameplay/lua/LuaAPI.hpp"
#include "gameplay/lua/ProcessCoroutine.hpp"
#include "gameplay/StageSceneNode.hpp"
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
        static void createMathTypes(sol::state& l);

    private:
        void startMainFunction();

        sol::state L;
        LuaAPI m_api;
        std::shared_ptr<ProcessCoroutine> m_stageCoro;
    };
}

