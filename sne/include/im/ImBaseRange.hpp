#pragma once

/// @brief Helper class to implement for floating points types to draw as a slider with min and max values.
template<typename CRTP, typename T>
class ImBaseRange
{
public:
    /// @param slider_ If true, enable the slider.
    CRTP& setRange(T min_, T max_, bool slider_ = true)
    {
        min = min_;
        max = max_;
        slider = slider_;
        return static_cast<CRTP&>(*this);
    }

    T min = {};
    T max = {};

    /// @brief To show the slider or not for the input (e.g. ImGui::InputFloat or ImGui::SliderFloat)
    /// @remarks False by default, so the slider is not enabled.
    bool slider = false;
};