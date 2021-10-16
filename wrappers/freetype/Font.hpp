#pragma once

#include "wrappers/freetype/FontType.hpp"
#include "private/DynamicRectPacker.hpp"
#include <wrappers/freetype/Glyph.hpp>
#include <filesystem>
#include <memory>
#include <SDL_version.h>

namespace FT
{
    class Face;

    namespace RAII
    {
        class Library;
    }
}

/// @brief Freetype wrappers, also GL wrappers.
/// @details Renderable freetype font in OpenGL.
class Font
{
public:
    using Type = FontType;

    /// @brief Creates an invalid font
    Font();

    /// @brief Creates a font from a font file.
    Font(std::filesystem::path fontPath);
    Font(Font&&) noexcept;
    Font& operator=(Font&&) noexcept;

    ~Font();

    /// @brief Get the version of freetype.
    /// @returns The FreeType version. We use a SDL_version structure to store the version because it has the same fields.
    static SDL_version getFreetypeCompiledVersion();
    SDL_version getFreetypeLinkedVersion() const;

    /// @brief Get the face of this type.
    /// @details The face of this type is loaded if it is the first use.
    [[nodiscard]] const FT::Face& getFace(FontType type) const;

private:
    /// @remarks unique_ptr for pImpl
    std::unique_ptr<FT::RAII::Library> m_library;

    /// @brief One font contains multiple face: one for each character size and one for outline/non-outline.
    mutable std::map<FontType, std::unique_ptr<FT::Face>> m_faces;

    std::filesystem::path m_fontPath;
};

