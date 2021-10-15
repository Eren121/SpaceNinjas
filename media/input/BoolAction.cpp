#include "BoolAction.hpp"

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
    });
    
    m_connections.emplace_back(conn);
}

void BoolAction::onUpdate(Key, bool pressed, long frame)
{
    update(pressed ? Max : 0, frame);
}
