#pragma once

#include "wrappers/gl/Texture.hpp"
#include <snk/binpacking/MaxRectsBinPack.h>
#include <snk/Rect.hpp>
#include <glm/vec2.hpp>
#include <optional>

/// @brief Utility class to pack textures.
/// @details Designed for fonts. You can add textures dynamically, but the position and size of textures never change.
/// Adapted to load characters only when needed.

class FontPacker
{
public:
    using rect_id = int;

    FontPacker(glm::ivec2 maxSize);

    const Texture& getTexture() const;

    /// @brief Insert a rect into the texture.
    /// @returns The Rect UV, or nullopt if the FontPacker is full, no space was found to add the texture.
    std::optional<Rect> insert(glm::ivec2 size, const void *pixels);

private:
    rbp::MaxRectsBinPack m_bin;
    Texture m_texture;
};