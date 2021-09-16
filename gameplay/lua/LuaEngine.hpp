#pragma once

#include "LuaAPI.hpp"
#include "LuaStageCoroutine.hpp"

class Stage;

/// @brief Manage the Stage scripts
class LuaEngine : public Process
{
public:
    /// @brief Run the associated script to the Stage.
    explicit LuaEngine(Stage& stage);
    
    /// @brief Run a script step.
    void update();
    
private:
    void initMain();
    
    sol::state m_L;
    Stage& m_stage;
    LuaAPI m_api;
    std::shared_ptr<LuaStageCoroutine> m_stageCoro;
};

