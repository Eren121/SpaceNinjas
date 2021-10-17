#pragma once

#include "media/Window.hpp"
#include <wrappers/gl/RenderStates.hpp>
#include <wrappers/gl/Shape.hpp>
#include <wrappers/gl/ConvexShape.hpp>
#include <wrappers/gl/Circle.hpp>
#include <wrappers/freetype/Text.hpp>

namespace SpaceNinja::test
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