#pragma once

#include "media/Window.hpp"
#include "media/scene/SceneNode.hpp"
#include <utility/time/FPSCounter.hpp>
#include <utility/time/Clock.hpp>

class WindowDebugInfo : public SceneNode
{
public:
    WindowDebugInfo(const Window& window);

protected:
    bool updateNode() override;
    void debugNode() override;
    
private:
    const Window& m_window;
    FPSCounter m_fpsCounter;
    Clock m_sinceOpen;
};

