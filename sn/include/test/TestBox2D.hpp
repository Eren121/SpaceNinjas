#pragma once

#include "Test.hpp"
#include <b2/World.hpp>
#include <b2/DebugDraw.hpp>

namespace sn::test
{
    /// @brief Test transformable with a IMGUI interface
    class TestBox2D : public Test
    {
    public:
        TestBox2D();

    protected:
        void draw() override;

    private:
        b2Body *b1;
        b2::World m_world;
        b2::DebugDraw m_debugDraw;
    };

}