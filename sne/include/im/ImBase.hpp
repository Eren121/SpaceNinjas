#pragma once

#include <string>
#include <imgui.h>

/// @tparam T The value type.
/// @remarks Guaranteed to have an unique ImGUI identifier per instance (based on class address).
template<typename CRTP, typename T>
class ImBase
{
public:
    /// @brief Make an instance.
    static CRTP make()
    {
        return {};
    }

    /// @remarks Set also the current value to the new default.
    CRTP& setDefault(T def_)
    {
        def = def_;
        value = def_;
        return static_cast<CRTP&>(*this);
    }

    CRTP& setLabel(const std::string& label_)
    {
        label = label_;
        return static_cast<CRTP&>(*this);
    }

    CRTP& setTooltip(const std::string& tooltip_)
    {
        tooltip = tooltip_;
        return static_cast<CRTP&>(*this);
    }

    CRTP& setReadonly(bool readonly_ = true)
    {
        readonly = readonly_;
        return static_cast<CRTP&>(*this);
    }

    /// @brief Show the ImGUI element with a "Reset" button.
    /// @returns true if the value has changed.
    /// @remarks Also wraps the label into an unique ID (the address of this instance).
    bool show()
    {
        const T previousValue = value;

        ImGui::PushID(this);

        if(readonly)
        {
            ImGui::BeginDisabled();
        }

        showResetButton();
        showImpl();

        if(readonly)
        {
            ImGui::EndDisabled();
        }

        showTooltip();
        ImGui::PopID();

        const bool wasModified = (previousValue != value);
        return wasModified;
    }

    T def = {}; ///< Default value
    T value = {}; ///< Current value
    std::string label; ///< ImGUI label
    std::string tooltip; ///< ImGUI tooltip
    bool readonly = false; ///< If true, the value cannot be modified by the user

protected:
    ImBase() = default;

    /// @brief Show the ImGUI element with a "Reset" button.
    virtual void showImpl() = 0;

private:
    /// @brief Show a tooltip if the tooltip string is not empty.
    /// @remarks Should be called just after having rendered the input field to have a tooltip for this input.
    void showTooltip() const
    {
        // ImGui still displays rectangular bounds if the string is empty,
        // so we check the tooltip is not empty before drawing it

        if(!tooltip.empty()) {

            // We don't display the tooltip on the label,
            // Because readonly is implemented with BeginDisabled()
            // And BeginDisabled() also disable the tooltip
            // And also it's more explicit help
            ImGui::SameLine();
            ImGui::TextDisabled("(?)");

            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip(tooltip.c_str());
            }
        }
    }

    void showResetButton()
    {
        // Show reset button only if the element is not read-only
        if(!readonly) {
            if (ImGui::Button("Reset")) {
                value = def;
            }

            ImGui::SameLine();
        }
    }
};
