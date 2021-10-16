#pragma once

#include "media/scene/SceneNode.hpp"
#include "Input.hpp"
#include "process/CoProcess.hpp"
#include <SDL_events.h>

class Game;

namespace ui
{
    /// @brief A vertical list, equally spaced, of options.
    class VerticalListMenu : public SceneNode, private CoProcess
    {
    public:
        /// @brief Constructor.
        explicit VerticalListMenu(Game& game);
    
        /// @brief Add an option to the menu.
        void addOption(const std::string& label, std::function<void()> onClick = {});

    protected:
        /// @brief Drawable implementation.
        /// @details The matrix should be in pixel coordinates.
        void drawNode(RenderStates states) const override;
        void debugNode() override;
        bool updateNode() override;
        
        task<> coroutine() override;

    private:
        void onClick();
        
        Game& m_game;
        /// @brief List of options of the menu.
        /// @details Rendered in the index order.
        std::vector<Input> m_inputs;
    
        enum {
            Unfocused = -1
        };
        
        /// @brief Currently focused element
        /// @remarks -1 if no element is focused.
        int m_focus {Unfocused};
    };
}