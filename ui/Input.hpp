#pragma once

#include <string>
#include <functional>

namespace SpaceNinja::ui
{
    /// @brief Generic UI element.
    /// @details It can be a button, a checkbox, a radio, etc...
    struct Input
    {
        std::string label;
        std::function<void()> onClick;
    };
}