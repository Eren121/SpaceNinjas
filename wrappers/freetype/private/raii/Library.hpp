#pragma once

#include <snk/swapmove.hpp>
#include <SDL_version.h>
#include <ft2build.h>
#include FT_FREETYPE_H

/// @brief Utilities for FreeType, notably RAII types.
namespace FT::RAII
{
    /// @brief RAII FT_Library.
    class Library
    {
    public:
        /// @brief Creates the library.
        Library();

        /// @brief Destroys the library.
        ~Library() noexcept(false);

        /// @brief Make moveable, but not copyable.
        SWAP_MOVABLE(Library, &Library::m_wrapped)

        /// @brief Conversion to wrapped type.
        operator FT_Library() const;

        /// @name Get the version of Freetype.
        /// @{
        static SDL_version getCompiledVersion();
        [[nodiscard]] SDL_version getLinkedVersion() const;
        /// @}

    private:
        FT_Library m_wrapped{nullptr};
    };
}