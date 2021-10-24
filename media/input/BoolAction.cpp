#include "BoolAction.hpp"

namespace Snow::in
{
    BoolAction::BoolAction()
    {
        // Dead zone is always zero for bool because values are only either 0 or 1
        // With any dead zone > 0 it would be always 0
        m_deadZone = 0;
    }

    void BoolAction::bindKey(Key key)
    {
        m_input->onKeyChanged[key].connect(&BoolAction::onUpdate, this);
    }

    void BoolAction::bindAxis(SDL_GameControllerAxis axis, float deadZone)
    {
        auto conn = m_input->onAxisChanged[axis].connect([this, deadZone]
            (SDL_GameControllerAxis, int value, long frame, SDL_GameController *) {
                update(applyDeadZone(value, deadZone) ? Max : 0, frame);
            }
        );

        m_connections.emplace_back(conn);
    }

    void BoolAction::onUpdate(Key, bool pressed, long frame)
    {
        update(pressed ? Max : 0, frame);
    }

    void BoolAction::bindMouse(int button)
    {
        auto conn1 = m_input->onEventType[SDL_MOUSEBUTTONDOWN].connect(
        [this, button](const UnifiedInput::Source &source) {
                if(source.event.button.button == button)
                {
                    update(Max, source.frame);
                }
            }
        );

        auto conn2 = m_input->onEventType[SDL_MOUSEBUTTONUP].connect(
        [this, button](const UnifiedInput::Source &source) {
                if(source.event.button.button == button)
                {
                    update(0, source.frame);
                }
            }
        );

        m_connections.emplace_back(conn1);
        m_connections.emplace_back(conn2);
    }
}