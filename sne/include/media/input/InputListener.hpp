#pragma once

#include "Key.hpp"
#include <sigslot/signal.hpp>
#include <unordered_map>

namespace Snow::media
{
    class Window;
}

namespace Snow::in
{
    class InputListener : public sigslot::observer
    {
    public:
        struct Source
        {
            const SDL_Event &event; ///< What event happened?
            long frame; ///< At which frame this event happened?
        };

        explicit InputListener(media::Window &window);

        virtual ~InputListener() = default;

        /// @remarks long parameter is the frame
        std::unordered_map<Key, sigslot::signal<Key, bool, long>> onKeyChanged;
        std::unordered_map<SDL_GameControllerAxis, sigslot::signal<SDL_GameControllerAxis, int, long, SDL_GameController *>> onAxisChanged;

        /// @brief Listener to any event type.
        /// @remarks the long argument is the frame.
        std::unordered_map<SDL_EventType, sigslot::signal<Source>> onEventType;

        media::Window &getWindow();

    private:
        void onEvent(const SDL_Event &event, long frame);

    protected:
        media::Window &m_window;
    };
}