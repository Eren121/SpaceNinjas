#pragma once

#include <wrappers/gl/Texture.hpp>
#include <glm/vec2.hpp>

namespace FT
{
    class Face;
}
struct Glyph
{
    char c{'\0'}; ///< Character the Glyph represents

    const Texture *texture{nullptr}; ///< Texture containing the character on the font

    Rect uv; ///< UV coordinates on the texture of the page

    glm::vec2 size{0.0f}; ///< Size of the glyph in pixel

    glm::vec2 bearing{0.0f}; ///< Bearing in pixel
    float advance{0.0f}; ///< Advance, in pixel
};

