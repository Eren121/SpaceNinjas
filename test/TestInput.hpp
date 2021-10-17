#pragma once

#include "test/Test.hpp"
#include "media/Window.hpp"
#include "wrappers/gl/Shader.hpp"
#include "wrappers/freetype/Font.hpp"

namespace SpaceNinja::test
{

/// @brief Test transformable with a IMGUI interface
class TestInput : public Test
{
public:
    TestInput();

protected:
    void draw() override;

private:
    SDL::GameController m_gameController;
    std::string m_lastEvent;

    Snow::in::BoolAction shoot;
    Snow::in::AxisAction jump;
    Snow::in::VectorAction move;
};

}