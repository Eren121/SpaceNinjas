#pragma once

#include "media/input/BoolAction.hpp"
#include "media/input/VectorAction.hpp"
#include "media/scene/SceneNode.hpp"
#include "media/SDL.hpp"

namespace sn
{
class GameControls : public SceneNode
{
public:
    explicit GameControls(Snow::in::UnifiedInput &input);

    void debugNode() override;

    Snow::in::BoolAction shoot;
    Snow::in::VectorAction move;
    Snow::in::VectorAction turn;

    Snow::in::VectorAction menuMove;
    Snow::in::BoolAction menuSelect;
    Snow::in::BoolAction menuBack;

    Snow::in::BoolAction showDebugWindow;

    Snow::in::BoolAction switchFullscreen;

private:
    void listenGameControllerChange(Snow::in::UnifiedInput &input);

    SDL::GameController m_gameController;
    [[maybe_unused]] std::vector<sigslot::scoped_connection> m_connections;
};
}