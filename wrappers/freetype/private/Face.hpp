#pragma once

#include "../Glyph.hpp"
#include "FontPacker.hpp"
#include "raii/Face.hpp"
#include <filesystem>

namespace FT
{
    class Face
    {
    public:
        /// @brief Creates an invalid Face
        Face() = default;
        explicit Face(FT_Library library, const std::filesystem::path& path, int characterSize);

        virtual ~Face() = default;

        /// @brief Get the texture at this page.
        /// @details The page should exist.
        [[nodiscard]] const Texture& getPage(int page) const;

        /// @brief Get a glyph, load the glyph if the glyph was not already loaded.
        /// @param c The glyph of this character to get.
        const Glyph& getGlyph(char c) const;

        /// @brief Get the line height in pixel for this font face.
        [[nodiscard]] float getLineHeight() const;

        /// @name
        /// @brief Get the ascender/descender of the font face, with the difference that it is in pixels and not
        /// in font units like with Freetype.
        /// @details Definition of ascender/descender is on Freetype documentation.
        /// @{
        [[nodiscard]] float getAscender() const;
        [[nodiscard]] float getDescender() const;
        /// @}


        /// @brief Convert font unit to pixels.
        /// @param x, y Font units that are converted to pixels.
        /// @details If you want only x or only y, you can pass zero to the non desired coordinate.
        void convertFontUnitToPixel(float *x, float *y) const;

    protected:
        virtual void loadGlyph(char c) = 0;

        /// @brief Register a glyph
        /// @details Should be called from loadGlyph().
        /// @param bitmap The image for this glyph
        /// @param c The character of this glyph
        /// @param bearing, advance The Freetype parameters of this glyph
        void registerGlyph(FT_Bitmap &bitmap, char c, const glm::vec2& bearing, float advance);

    private:

        /// @brief Add a new page in back to the registry
        void addPage();

    protected:
        RAII::Face m_face;

    private:
        /// @remarks Vector of unique_ptr to prevent invalidation of pointers if realloc'ed.
        std::vector<std::unique_ptr<FontPacker>> m_pages;

        /// @brief Map a character to its glyph.
        std::map<char, Glyph> m_registry;

        /// @brief Count of pixel for each texture page
        glm::ivec2 m_pageSize {1024};
    };

    class FaceSolid : public Face
    {
    public:
        using Face::Face;

    protected:
        void loadGlyph(char c) override;
    };

    class FaceOutline : public Face
    {
    public:
        FaceOutline() = default;
        explicit FaceOutline(FT_Library library, const std::filesystem::path& path, int characterSize);

    protected:
        void loadGlyph(char c) override;

        RAII::Stroker m_stroker;
    };
}