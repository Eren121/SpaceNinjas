#pragma once

#include "media/Window.hpp"
#include "wrappers/gl/Shader.hpp"
#include "wrappers/freetype/Font.hpp"
#include <snk/logging.hpp>

namespace SpaceNinja::test
{
    /// @brief Test transformable with a IMGUI interface
    class Test
    {
        LOGGABLE("Test")

    public:
        Test();

        virtual ~Test() = default;

        void run();

        static void runTests();

    protected:
        virtual void draw() = 0;

    protected:
        Font m_font;
        Snow::media::Window m_window;
        Shader m_shader;
    };

}