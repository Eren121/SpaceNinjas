#pragma once

#include "AxisAction.hpp"
#include <glm/vec2.hpp>

namespace Snow::in
{
class VectorAction
{
public:
    /// @remarks You should set an input before binding any value
    void setInput(UnifiedInput *input);

    void bindAxes(SDL_GameControllerAxis axis_x, SDL_GameControllerAxis axis_y, float deadZone = 0.15f);

    void bindKeys(Key x_minus, Key x_plus, Key y_minus, Key y_plus);

    glm::vec2 getValue() const;

private:
    UnifiedInput *m_input{nullptr};

    AxisAction m_actionX;
    AxisAction m_actionY;
};
}

