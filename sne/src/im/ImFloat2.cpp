#include "im/ImFloat2.hpp"

void ImFloat2::showImpl()
{
    const char* const label_ = label.c_str();
    float *const value_ = &value.x;
    const std::string format = buildFormat();

    if(slider) {
        ImGui::SliderFloat2(label_, value_, min, max, format.c_str());
    }
    else {
        ImGui::InputFloat2(label_, value_, format.c_str());
    }
}
