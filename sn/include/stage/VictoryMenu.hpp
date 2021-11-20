#pragma once

#include "Game.hpp"
#include "Victory.hpp"
#include <process/CoProcess.hpp>
#include <media/Window.hpp>

/// @brief Process to show the end stage menu.
class VictoryMenu : public Snow::exe::CoProcess
{
public:
    /// @param stage the stage that just finished.
    VictoryMenu(std::shared_ptr<sn::Stage> stage);

protected:
    task<> coroutine() override;

    task<> fadeInVictoryMessage();
    task<> goToNextLevel();

private:
    sn::Game& m_game;
    Snow::media::Window& m_window;
    std::shared_ptr<sn::Stage> m_stage;
};