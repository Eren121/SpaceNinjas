#include "FontPacker.hpp"

FontPacker::FontPacker(glm::ivec2 maxSize)
    : m_bin(maxSize.x, maxSize.y, false) // Do not allow flip to simplify management
{
    Texture::bind(&m_texture);
    m_texture.setFilter(Texture::Linear);

    // Only red channel is used, but we want to render as the alpha, with full white
    glm::ivec4 swizzle = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzle.r);


    // Clamping is important for linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glm::vec4 border = {0.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border.r);

    // Important to filter,
    // Because we can generate one high-resolution font and use it for texts of smaller size and interpolate
    // Otherwise NEAREST will be ugly because each pixel is either transparent or opaque.

    m_texture.load(maxSize, {0.0f, 0.0f, 0.0f, 0.0f});
}

const Texture& FontPacker::getTexture() const
{
    return m_texture;
}

std::optional<Rect> FontPacker::insert(glm::ivec2 size, const void *pixels)
{
    if(size.x == 0 || size.y == 0)
    {
        // Size zero requested, returns a rect of size zero
        return Rect{};
    }

    Recti ret;
    {
        // Add 1px of border
        // Because if the texture is interpolated (magnify/minify), with linear interpolation,
        // neighbors may be used for rendering but we don't want glyphs colors to interfere between each other
        int border = 1; // border each side in px

        rbp::Rect inserted = m_bin.Insert(size.x + border * 2, size.y + border * 2, rbp::MaxRectsBinPack::RectBestAreaFit);

        if(inserted.height == 0)
        {
            // Not enough space
            return std::nullopt;
        }

        ret.origin = {inserted.x, inserted.y};
        ret.origin += border;

        ret.size = {inserted.width, inserted.height};
        ret.size -= border * 2;
    }

    // disable byte-alignment restrictiont
    // texture should be multiple of 4 by default, but glyphs are greyscale so it could be of any size (= multiple of 1)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    m_texture.load(ret, pixels, GL_RED, GL_UNSIGNED_BYTE);

    Rect uv;
    uv.origin = glm::vec2(ret.origin) / m_texture.getSize();
    uv.size = glm::vec2(ret.size) / m_texture.getSize();

    return uv;
}
