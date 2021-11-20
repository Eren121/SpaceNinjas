#include "stage/VictoryMenu.hpp"
#include <media/Window.hpp>
#include <str.hpp>
#include <process/Wait.hpp>
#include <process/ProcessQueue.hpp>
#include <ft/Text.hpp>

#include <glm/gtc/matrix_transform.hpp>

VictoryMenu::VictoryMenu(std::shared_ptr<sn::Stage> stage)
        : m_game(stage->getGame()),
          m_window(m_game.getWindow()),
          m_stage(stage)
{
}

task<> VictoryMenu::coroutine()
{
    if(m_stage->getVictory() == Victory::Loss)
    {
        // Loss, restart the level immediately
        m_game.pushSceneStage(m_stage->getID());
    }
    else
    {
        // Win, show win screen
        // Then go to next Stage

        co_await fadeInVictoryMessage();
        co_await goToNextLevel();
    }
}

task<> VictoryMenu::fadeInVictoryMessage()
{

    // Draw text on center of the screen

    Text info;
    info.setCharacterSize(128);
    info.setFont(&m_game.getFont());
    info.setString(snk::str{} << "Victory Stage " << m_stage->getID());
    info.setOrigin(info.getSize() / 2.0f);
    info.setPosition(m_window.getSize() * glm::vec2{0.5f, 0.75f});
    info.setOutlineColor({1.0f, 0.0f, 0.0f, 1.0f});
    info.setOutline(true);

    RenderStates states;
    states.shader = &m_game.getShader();
    states.view = glm::ortho(0.0f, m_window.getSize().x, 0.0f, m_window.getSize().y);

    // Show shade in Victory
    Snow::exe::Wait wait(Time::seconds(5));
    while (wait())
    {
        float opacity = wait.getProgress();

        info.setOpacity(opacity);
        info.draw(states);

        co_await std::suspend_always{};
    }
}

task<> VictoryMenu::goToNextLevel()
{
    m_game.pushSceneStage(m_stage->getID() + 1);
    co_return;
}