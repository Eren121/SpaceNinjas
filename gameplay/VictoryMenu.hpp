#pragma once

#include "Game.hpp"
#include "Victory.hpp"
#include "process/CoProcess.hpp"

/// @brief Process to show the end stage menu.
class VictoryMenu : public CoProcess
{
public:
    /// @param stage the stage that just finished.
    VictoryMenu(std::shared_ptr<Stage> stage);

protected:
    task<> coroutine() override;

    task<> fadeInVictoryMessage();
    task<> goToNextLevel();

private:
    Game& m_game;
    Window& m_window;
    std::shared_ptr<Stage> m_stage;
};