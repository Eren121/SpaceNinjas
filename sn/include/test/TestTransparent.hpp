#pragma once

#include <media/Window.hpp>
#include <gl/RenderStates.hpp>
#include <gl/Shape.hpp>
#include <gl/ConvexShape.hpp>
#include <gl/Circle.hpp>
#include <ft/Text.hpp>

namespace sn::test
{

class TestTransparent
{
public:
    TestTransparent();

    void run();

protected:
    void draw();

private:
    Snow::media::Window m_window;
    Shader m_shader;
};

}