#pragma once

#include "input/UnifiedInput.hpp"
#include <wrappers/SDL.hpp>
#include <wrappers/DebugWindow.hpp>
#include <wrappers/gl/Texture.hpp>
#include <sigslot/signal.hpp>
#include <glm/glm.hpp>
#include <string>

namespace Snow::media
{
    /// @brief Window instance.
    class Window
    {
    public:

        enum FullscreenState
        {
            Windowed = 0, ///< Not fullscreen
            Fullscreen, ///< True fullscreen
            BorderlessFullscreen, /// "Fake" fullscreen (borderless window with maximum size)

            Count /// Do not use, to count enum values
        };

        Window(const std::string& title, int width, int height);

        /// @brief Deletes the window and the OpenGL context
        ~Window();

        /// @returns False when user want to quit the window (ALT+F4, icon, etc...).
        bool isOpen() const;
        void close();

        /// @brief Handle the window events.
        /// @remarks Call it one time per frame, at the beginning of the frame.
        void handleEvents();

        /// @brief Render the window by swapping OpenGL buffers.
        void display();

        /// @brief Get the size of the window in pixel.
        glm::vec2 getSize() const;

        in::UnifiedInput& getInput();
        const in::UnifiedInput& getInput() const;

        SDL_Window* getHandle() const;

        long getFrame() const;

        /// @name
        /// @brief Fullscreen mode
        /// @{
        FullscreenState getFullscreenState() const;
        void setFullscreenState(FullscreenState state);
        /// @}

        SDL_Window* getHandle();

    private:
        /// @brief Setup the options for OpenGL.

        static void setupGLAttributes();

        /// @brief Creates the window
        void createWindow(const std::string& title, int width, int height);

        /// @brief Creates the OpenGL context
        void createContext();

        /// @brief Initialize GLEW (should be called after having created the OpenGL context)
        static void initGLEW();

        SDL_Window *m_window;
        SDL_GLContext m_context;
        bool m_running;

    public:
        /// @brief Called when each event is intercepted.
        /// @remarks The long parameter is the frame
        sigslot::signal<const SDL_Event&, long> onEvent;

        /// @brief Called at each frame.
        sigslot::signal<> onNewFrame;

    private:
        std::unique_ptr<DebugWindow> m_debugWindow;

        static void printGPUInfo();

        in::UnifiedInput m_input;
        long m_frame{0};
    };
}