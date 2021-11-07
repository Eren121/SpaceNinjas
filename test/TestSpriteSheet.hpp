#pragma once

#include "Test.hpp"
#include "media/ase/SpriteSheet.hpp"

namespace SpaceNinja::test
{
    /// @brief Test transformable with a IMGUI interface
    class TestSpriteSheet : public Test
    {
    public:
        TestSpriteSheet();

    protected:
        void draw() override;

    private:
        Snow::media::ase::SpriteSheet m_sheet;
    };

}