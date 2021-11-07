#include "TestSpriteSheet.hpp"
#include "wrappers/gl/Sprite.hpp"
#include <iostream>

namespace SpaceNinja::test
{
    TestSpriteSheet::TestSpriteSheet()
        : m_sheet{"../assets/test/explosion-1.json"}
    {
    }

    void TestSpriteSheet::draw()
    {
        static bool first = true;
        static Texture texture;
        static Sprite sprite{};
        static int index = 0;

        if(first)
        {
            first = false;
            texture.load(m_sheet.getFrames()[0].imagePath);

            // Artifacts in linear filtering due to texture atlas
            texture.setFilter(Texture::Nearest);

            sprite.setTexture(&texture);

            sprite.setScale(glm::vec2{1.0f, 1.0f}/4.0f);
        }

        RenderStates states;
        states.shader = &m_shader;

        //Rect rect{m_sheet.getFrames()[index].rect};
        Rect rect{m_sheet.getFrameFromTime(Time::now() / 5.0f).rect};

        std::cout<<to_string(texture.getSize())<<std::endl;
        rect /= glm::vec2{texture.getSize()};

        sprite.setUVs(rect);

        sprite.draw(states);

        std::cout << index << ": " << to_string(rect) << std::endl;

        index++;
        if(index >= m_sheet.getFrames().size()) index = 0;
    }
}