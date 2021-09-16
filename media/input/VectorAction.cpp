#include "VectorAction.hpp"

void VectorAction::setInput(UnifiedInput *input)
{
    m_input = input;
    m_actionX.setInput(input);
    m_actionY.setInput(input);
}

void VectorAction::bindAxes(SDL_GameControllerAxis axis_x, SDL_GameControllerAxis axis_y, float deadZone)
{
    m_actionX.bindAxis(axis_x, deadZone);
    m_actionY.bindAxis(axis_y, deadZone, true); // Reverse Y because SDL origin is top-left
}

void VectorAction::bindKeys(Key x_minus, Key x_plus, Key y_minus, Key y_plus)
{
    m_actionX.bindKeys(x_minus, x_plus);
    m_actionY.bindKeys(y_minus, y_plus);
}

glm::vec2 VectorAction::getValue() const
{
    return {m_actionX.asFloat(), m_actionY.asFloat()};
}
