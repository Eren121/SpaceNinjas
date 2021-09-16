#pragma once

#include <SDL2/SDL.h>

/// @brief ImGUI with OpenGL and SDL wrapper
class DebugWindow
{
public:
    /// @brief Instanciate it after having initialized the OpenGL context.
    DebugWindow(SDL_Window *window);
    ~DebugWindow();

    DebugWindow(const DebugWindow&) = delete;
    DebugWindow& operator=(const DebugWindow&) = delete;

    void onFrameStarted();
    void onEvent(const SDL_Event& e);
    void draw();
};