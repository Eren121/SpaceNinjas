#include "im/ImTime.hpp"

void ImTime::showImpl()
{
    const float multiplier = findMultiplier(unit);
    const float min_ = min.asSeconds() * multiplier;
    const float max_ = max.asSeconds() * multiplier;
    const std::string format = buildFormat();

    float amount = value.asSeconds() * multiplier;

    if(slider) {
        ImGui::SliderFloat(label.c_str(), &amount, min_, max_, format.c_str());
    }
    else {
        ImGui::InputFloat(label.c_str(), &amount, min_, max_, format.c_str());
    }

    // Divide, because: if for example unit is millisecond
    // The input field is "100", then it means 0.1s => so divide by 1000

    value = Time::seconds(amount / multiplier);
}
