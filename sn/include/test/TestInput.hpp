#pragma once

#include "Test.hpp"
#include "GameControls.hpp"
#include <media/Window.hpp>
#include <gl/Shader.hpp>
#include <ft/Font.hpp>

namespace sn::test
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