#pragma once

#include <string>
#include <functional>
#include "Fwd.hpp"

namespace sn::ui
{
    using Callback = std::function<void(SceneNode&)>;

    /// @brief Generic UI element.
    /// @details It can be a button, a checkbox, a radio, etc...
    struct Input
    {
        explicit Input(const std::string& label, Callback onClick)
            : label(label), onClick(onClick) {}

        std::string label;
        Callback onClick;
    };
}