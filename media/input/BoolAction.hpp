#pragma once

#include "InputListener.hpp"
#include "Action.hpp"

namespace Snow::in
{
class BoolAction : public Action
{
public:
    BoolAction();

    void bindKey(Key key);

    void bindAxis(SDL_GameControllerAxis axis, float deadZone = 0.15f);

private:
    void onUpdate(Key key, bool pressed, long frame);

    [[maybe_unused]] std::vector<sigslot::scoped_connection> m_connections;
};
}
