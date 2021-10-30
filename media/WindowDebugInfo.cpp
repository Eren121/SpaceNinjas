#include "WindowDebugInfo.hpp"
#include "Window.hpp"
#include <imgui.h>

namespace Snow::media
{
    WindowDebugInfo::WindowDebugInfo(const Window &window)
            : m_window(window)
    {

    }

    bool WindowDebugInfo::updateNode()
    {
        m_fpsCounter.onFrame();

        return true;
    }

    void WindowDebugInfo::debugNode()
    {
        if (ImGui::CollapsingHeader("Window"))
        {
            float secSinceOpen = m_sinceOpen.getElapsedTime().asSeconds();
            float fps = m_fpsCounter.getFPS();
            int frame = static_cast<int>(m_window.getFrame());
            glm::ivec2 winSize = m_window.getSize();

            ImGui::InputFloat("Average FPS", &fps, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
            ImGui::InputInt("Current frame", &frame, 0, 0, ImGuiInputTextFlags_ReadOnly);
            ImGui::InputInt2("Window size", &winSize.x, ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("Time since open", &secSinceOpen, 0, 0, "%.3fs", ImGuiInputTextFlags_ReadOnly);
        }
    }
}