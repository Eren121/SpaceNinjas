#include "Save.hpp"
#include <imgui.h>

/// @brief Log a change when a field change
#define SAVE_UPDATE_FIELD(field, newValue) do { getLogger().debug("Set {}: old={}, new={}", &#field[2], field, newValue); \
    field = newValue; } while(0)

void Save::debugNode()
{
    if(ImGui::CollapsingHeader("Save"))
    {
        ImGui::InputInt("Last level unlocked", &m_lastLevelUnlocked);
    }
}

int Save::getLastLevelUnlocked() const
{
    return m_lastLevelUnlocked;
}

void Save::setLastLevelUnlocked(int i)
{
    SAVE_UPDATE_FIELD(m_lastLevelUnlocked, i);
}