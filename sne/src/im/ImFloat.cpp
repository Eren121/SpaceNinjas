#include "im/ImFloat.hpp"
#include <imgui.h>

void ImFloat::showImpl()
{
    const char* const label_ = label.c_str();
    const std::string format = buildFormat();

    if(slider) {
        ImGui::SliderFloat(label_, &value, min, max, format.c_str());
    }
    else {
        ImGui::InputFloat(label_, &value, min, max, format.c_str());
    }
}