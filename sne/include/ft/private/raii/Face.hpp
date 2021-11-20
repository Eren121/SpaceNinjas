#pragma once

#include "Stroker.hpp"
#include "../FontPacker.hpp"
#include "../../Glyph.hpp"
#include <swapmove.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <filesystem>
#include <map>

namespace FT::RAII
{
    /// @brief RAII FT_Face.
    struct Face
    {
    public:
        /// @brief Creates an invalid font face.
        Face() = default;

        /// @brief Loads a face.
        /// @param library Needed to load the font face. The library should live at least as long as the font face.
        /// @param path The path of the font face.
        /// @param characterSize The character size of the font face.
        Face(FT_Library library, const std::filesystem::path& path, int characterSize);

        /// @brief Destroys the font face.
        virtual ~Face() noexcept(false);

        /// @brief Make moveable, but not copyable.
        SWAP_MOVABLE(Face, &Face::m_wrapped)

        operator FT_Face() const;
        FT_Face operator->() const;

    private:
        FT_Face m_wrapped{nullptr};
    };
}