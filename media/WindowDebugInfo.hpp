#pragma once

#include "Window.hpp"
#include "scene/SceneNode.hpp"
#include <snk/time/FPSCounter.hpp>
#include <snk/time/Clock.hpp>

namespace Snow::media
{
/// @brief Utility node to display FPS.
class WindowDebugInfo : public SceneNode
{
public:
    WindowDebugInfo(const Window &window);

protected:
    bool updateNode() override;

    void debugNode() override;

private:
    const Snow::media::Window &m_window;
    FPSCounter m_fpsCounter;
    Clock m_sinceOpen;
};
}