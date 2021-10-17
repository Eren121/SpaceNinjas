#include <freetype/freetype.h>
#include "Action.hpp"
#include "media/Window.hpp"

namespace Snow::in
{

bool Action::isPressed() const
{
    return m_currentValue != 0;
}

bool Action::isJustPressed() const
{
    return isPressed() && m_lastFrameUpdate == m_input->getWindow().getFrame()
        && m_previousValue == 0;
}

bool Action::isJustReleased() const
{
    return !isPressed() && m_lastFrameUpdate == m_input->getWindow().getFrame()
        && m_previousValue != 0;
}

float Action::asFloat() const
{
    return asFloat(m_currentValue);
}

int Action::applyDeadZone(int value, float deadZone)
{
    if(std::abs(value) <= asInt(deadZone))
    {
        return 0;
    }
    else
    {
        return value;
    }
}

void Action::update(int newValue, long frame)
{
    if(newValue != m_currentValue)
    {
        // Only set the previous if its on different frame
        m_lastFrameUpdate = frame;
        m_previousValue = m_currentValue;
        m_currentValue = newValue;
    }
}

void Action::setInput(UnifiedInput *input)
{
    m_input = input;
}

float Action::asFloat(int intValue)
{
    return static_cast<float>(intValue) / static_cast<float>(Max);
}

int Action::asInt(float floatValue)
{
    return static_cast<int>(floatValue * static_cast<float>(Max));
}

}