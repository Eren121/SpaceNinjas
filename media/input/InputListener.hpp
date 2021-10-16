#pragma once

#include "Key.hpp"
#include <sigslot/signal.hpp>
#include <SDL_events.h>
#include <unordered_map>

class Window;

class InputListener : public sigslot::observer
{
public:
    struct Source
    {
        const SDL_Event &event;
        long frame;
    };
    
    explicit InputListener(Window& window);
    virtual ~InputListener() = default;
    
    /// @remarks long parameter is the frame
    std::unordered_map<Key, sigslot::signal<Key, bool, long>> onKeyChanged;
    std::unordered_map<SDL_GameControllerAxis, sigslot::signal<SDL_GameControllerAxis, int, long, SDL_GameController*>> onAxisChanged;
    
    /// @brief Listener to any event type.
    /// @remarks the long argument is the frame.
    std::unordered_map<SDL_EventType, sigslot::signal<Source>> onEventType;
    
    Window& getWindow();
    
private:
    void onEvent(const SDL_Event& event, long frame);
    
protected:
    Window& m_window;
};

