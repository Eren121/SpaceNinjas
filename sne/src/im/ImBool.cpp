#include "im/ImBool.hpp"
#include <imgui.h>

void ImBool::showImpl()
{
    ImGui::Checkbox(label.c_str(), &value);
}