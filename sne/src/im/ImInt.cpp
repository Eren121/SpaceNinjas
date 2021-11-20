#include "im/ImInt.hpp"

void ImInt::showImpl()
{
    const char* const label_ = label.c_str();

    if(slider) {
        ImGui::SliderInt(label_, &value, min, max);
    }
    else {
        ImGui::InputInt(label_, &value);
    }
}