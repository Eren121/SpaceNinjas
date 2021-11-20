#pragma once

#include "Input.hpp"
#include <media/scene/SceneNode.hpp>
#include <process/CoProcess.hpp>
#include "Game.hpp"
#include "Fwd.hpp"
#include <SDL_events.h>

namespace sn::ui
{
    /// @brief A vertical list, equally spaced, of options.
    class VerticalListMenu : public SceneNode, private Snow::exe::CoProcess
    {
    public:

        /// @brief Constructor.
        explicit VerticalListMenu(sn::Game &game);

        /// @brief Add an option to the menu.
        void addOption(const std::string &label, Callback onClick = {});

        /// @returns The count of options.
        int count() const { return static_cast<int>(m_inputs.size()); }

    protected:
        /// @brief Drawable implementation.
        /// @details The matrix should be in pixel coordinates.
        void drawNode(RenderStates states) const override;

        void debugNode() override;

        bool updateNode() override;

        task<> coroutine() override;

    private:
        void onClick();

        sn::Game &m_game;

        /// @brief List of options of the menu.
        /// @details Rendered in the index order.
        std::vector<Input> m_inputs;

        enum
        {
            Unfocused = -1
        };

        /// @brief Currently focused element
        /// @remarks -1 if no element is focused.
        int m_focus{Unfocused};
    };
}