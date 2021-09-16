#include "AxisAction.hpp"

void AxisAction::bindKey(Key key)
{
    auto conn = m_input->onKeyChanged[key].connect([this] (Key key, bool pressed, long frame) {
        update(pressed ? Max : 0, frame);
    });
    
    m_connections.emplace_back(conn);
}

void AxisAction::bindKeys(Key minus, Key plus)
{
    {
        auto conn = m_input->onKeyChanged[minus].connect([this, plus] (Key, bool pressed, long frame) {
            int value = 0;
            
            if(pressed)
            {
                value = Min;
            }
            else if(isKeyPressed(plus))
            {
                value = Max;
            }
            
            update(value, frame);
        });
        
        m_connections.emplace_back(conn);
    }
    {
        auto conn = m_input->onKeyChanged[plus].connect([this, minus] (Key, bool pressed, long frame) {
            int value = 0;
            
            if(pressed)
            {
                value = Max;
            }
            else if(isKeyPressed(minus))
            {
                value = Min;
            }
            
            update(value, frame);
        });
        
        m_connections.emplace_back(conn);
    }
}

void AxisAction::bindAxis(SDL_GameControllerAxis axis, float deadZone, bool reverse)
{
    auto conn = m_input->onAxisChanged[axis].connect([this, deadZone, reverse]
        (SDL_GameControllerAxis, int value, long frame, SDL_GameController*) {
            value = applyDeadZone(value, deadZone);
            
            if(reverse)
            {
                value = -value;
            }
            
            update(value, frame);
    });
    
    m_connections.emplace_back(conn);
}
