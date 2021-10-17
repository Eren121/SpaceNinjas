#include "test/Test.hpp"
#include <utility/math.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

namespace SpaceNinja::test
{

Test::Test()
        : m_window("Test", 800, 800)
{
    std::filesystem::path path = std::filesystem::current_path() / "../assets";

    m_shader.load(path / "vert.glsl", path / "frag.glsl");
    m_font = Font(path / "fonts/monofonto.ttf");
}

void Test::run()
{
    while (m_window.isOpen()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_window.handleEvents();

        draw();

        m_window.display();
    }

    exit(0);
}

}