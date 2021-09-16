#include "Window.hpp"
#include "utility/Str.hpp"
#include <GL/glew.h>
#include <iostream>

Window::Window(const std::string& title, int width, int height)
    : m_window(nullptr), m_context(nullptr), m_running(false), m_input(*this)
{
    setupGLAttributes();
    createWindow(title, width, height);
    createContext();

    // GLEW should be initialized after creating the window context!!!
    initGLEW();

    m_debugWindow = std::make_unique<DebugWindow>(m_window);
}

Window::~Window()
{
    SDL_DestroyWindow(m_window), m_window = nullptr;
    SDL_GL_DeleteContext(m_context), m_context = nullptr;
}

void Window::createWindow(const std::string& title, int width, int height)
{
    assert(!m_window);

    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;

    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
    if(!m_window)
    {
        throw SDL::Exception("Failed to create the window");
    }

    m_running = true;
}

void Window::createContext()
{
    m_context = SDL_GL_CreateContext(m_window);
    if(!m_context)
    {
        throw SDL::Exception("Failed to create an OpenGL context inside the window");
    }

    printGPUInfo();
}

void Window::initGLEW()
{
    glewExperimental = true;
    GLenum err = glewInit();

    if(err != GLEW_OK)
    {
        throw Exception(Str{} << "Glew initialization failed: " << glewGetErrorString(err));
    }
}

void Window::setupGLAttributes()
{
    // OpenGL version
    int major = 3;
    int minor = 3;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // version 3.1 core
}

bool Window::isOpen() const
{
    return m_running;
}

void Window::handleEvents()
{
    m_frame++;
    
    // Process all pending events in the event queue leaving the queue empty
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        switch(e.type)
        {
            case SDL_QUIT:
                m_running = false;
                break;
        }
        
        m_debugWindow->onEvent(e);
        onEvent(e, m_frame);
    }

    m_debugWindow->onFrameStarted();
}

void Window::display()
{
    onNewFrame();
    m_debugWindow->draw();

    SDL_GL_SwapWindow(m_window);
}

#include <glm/gtx/string_cast.hpp>

glm::vec2 Window::getSize() const
{
    int w, h;

    // May be different from SDL_GetWindowSize(), see https://wiki.libsdl.org/SDL_GetWindowSize
    SDL_GL_GetDrawableSize(m_window, &w, &h);

    return {w, h};
}

UnifiedInput& Window::getInput()
{
    return m_input;
}

const UnifiedInput& Window::getInput() const
{
    return m_input;
}

void Window::printGPUInfo()
{
    // Print some info

    int maxTexSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    std::cout << "Max. texture size: " << maxTexSize << "x" << maxTexSize << std::endl;

}

SDL_Window *Window::getHandle() const
{
    return m_window;
}

bool Window::isFullscreen() const
{
    Uint32 mode = SDL_GetWindowFlags(m_window);
    bool ret;

    if((mode & SDL_WINDOW_FULLSCREEN_DESKTOP) || (mode & SDL_WINDOW_FULLSCREEN))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}

void Window::setFullscreen(bool flag)
{
    Uint32 mode;

    if(flag)
    {
        mode = SDL_WINDOW_FULLSCREEN_DESKTOP; // "fake" fullscreen, allow fast alt-tab
    }
    else
    {
        mode = 0; // 0 == not fullscreen
    }

    if(SDL_SetWindowFullscreen(m_window, mode) != 0)
    {
        throw SDL::Exception("SDL_SetWindowFullscreen()");
    }
}

SDL_Window *Window::getHandle()
{
    return m_window;
}

long Window::getFrame() const
{
    return m_frame;
}

void Window::close()
{
    m_running = false;
}
