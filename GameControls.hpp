#pragma once

#include "media/input/BoolAction.hpp"
#include "media/input/VectorAction.hpp"
#include "media/scene/SceneNode.hpp"
#include "wrappers/SDL.hpp"

class GameControls : public SceneNode
{
public:
    explicit GameControls(UnifiedInput& input);
    
    void debugNode() override;
    
    BoolAction shoot;
    VectorAction move;
    VectorAction turn;
    
    VectorAction menuMove;
    BoolAction menuSelect;
    BoolAction menuBack;
    
    BoolAction showDebugWindow;
    
private:
    void listenGameControllerChange(UnifiedInput& input);
    
    SDL::GameController m_gameController;
    [[maybe_unused]] std::vector<sigslot::scoped_connection> m_connections;
};

