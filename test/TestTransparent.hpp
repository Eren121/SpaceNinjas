#include "media/Window.hpp"
#include <wrappers/gl/RenderStates.hpp>
#include <wrappers/gl/Shape.hpp>
#include <wrappers/gl/ConvexShape.hpp>
#include <wrappers/gl/Circle.hpp>
#include <wrappers/freetype/Text.hpp>

class TestTransparent
{
public:
    TestTransparent();

    void run();

protected:
    void draw();

private:
    Window m_window;
    Shader m_shader;
};