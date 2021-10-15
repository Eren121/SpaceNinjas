#include "Save.hpp"
#include <imgui.h>

void Save::debugNode()
{
    if(ImGui::CollapsingHeader("Save"))
    {
        ImGui::InputInt("Last level unlocked", &lastLevelUnlocked);
    }
}
