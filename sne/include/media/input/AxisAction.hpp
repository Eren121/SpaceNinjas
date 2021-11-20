#pragma once

#include "InputListener.hpp"
#include "Action.hpp"

namespace Snow::in
{
class AxisAction : public Snow::in::Action
{
public:
    void bindKey(Key key);

    void bindKeys(Key minus, Key plus);

    void bindAxis(SDL_GameControllerAxis axis, float deadZone = 0.15f, bool reverse = false);

private:
    [[maybe_unused]] std::vector<sigslot::scoped_connection> m_connections;
};
}
