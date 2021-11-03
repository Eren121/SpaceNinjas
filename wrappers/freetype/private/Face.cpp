#include "Face.hpp"
#include "Check.hpp"
#include "FT.hpp"
#include "wrappers/freetype/FTException.hpp"
#include <utility>

namespace FT
{
    Face::Face(FT_Library library, const std::filesystem::path& path, int characterSize)
        : m_face(library, path, characterSize)
    {
    }

    const Texture& Face::getPage(int page) const
    {
        return m_pages.at(page)->getTexture();
    }

    const Glyph& Face::getGlyph(char c) const
    {
        if(!m_registry.contains(c))
        {
            // mutable
            const_cast<Face&>(*this).loadGlyph(c);
        }

        return m_registry.at(c);
    }

    float Face::getLineHeight() const
    {
        // https://stackoverflow.com/questions/26486642/whats-the-proper-way-of-getting-text-bounding-box-in-freetype-2
        // 26.6 format: 1 unit = 1/64 pixel
        return m_face->size->metrics.height / 64.0f;
    }

    float Face::getAscender() const
    {
        float y = m_face->ascender;
        convertFontUnitToPixel(nullptr, &y);

        return y;
    }

    float Face::getDescender() const
    {
        float y = m_face->descender;
        convertFontUnitToPixel(nullptr, &y);

        return y;
    }

    void Face::convertFontUnitToPixel(float *x, float *y) const
    {
        // definition of x_scale, y_scale: 16.16 scale to convert font unit to 26.6 pixel
        float scale_x = m_face->size->metrics.x_scale;
        float scale_y = m_face->size->metrics.y_scale;

        scale_x /= 65536.0f; // convert scale from 16.16 to pixel unit
        scale_y /= 65536.0f;

        if(x)
        {
            float px_x = *x * scale_x;
            px_x /= 64.0f; // convert px from 26.6 to pixel
            *x = px_x;
        }
        if(y)
        {
            float px_y = *y * scale_y;
            px_y /= 64.0f;
            *y = px_y;
        }
    }

    void Face::registerGlyph(FT_Bitmap &bitmap, char c, const glm::vec2& bearing, float advance)
    {
        Glyph glyph;
        glyph.c = c; // Set the glyph fields to the parameters
        glyph.bearing = bearing;
        glyph.advance = advance;
        glyph.size = {bitmap.width, bitmap.rows};

        if(bitmap.pitch > 0)
        {
            // Negative pitch, need to reverse the texture because pixels are upside-down
            reverseBitmapVertically(bitmap);
        }

        // Load into the last page.
        // If this is the first glyph, the pages may be empty.
        if(m_pages.empty())
        {
            addPage();
        }

        // Try to insert the glyph. If there is not enough space, add a page
        std::optional<Rect> uvs = m_pages.back()->insert({bitmap.width, bitmap.rows}, bitmap.buffer);
        if(!uvs)
        {
            addPage();
            uvs = m_pages.back()->insert({bitmap.width, bitmap.rows}, bitmap.buffer);

            if(!uvs)
            {
                throw snk::exception("Unexpected error while trying to add a glyph texture page");
            }
        }

        // Set remaining parameters we got now
        glyph.texture = &m_pages.back()->getTexture();
        glyph.uv = *uvs;

        m_registry[c] = glyph;
    }

    void Face::addPage()
    {
        const glm::ivec2 pageSize {m_pageSize};
        m_pages.emplace_back(std::make_unique<FontPacker>(pageSize));
    }

    void FaceSolid::loadGlyph(char c)
    {
        // FT_LOAD_RENDER: pre-render the glyph in greyscale 8-bits => so we know a pixel is uint8 in range [0;255] luminance.
        FT_Check(FT_Load_Char(m_face, c, FT_LOAD_RENDER));

        auto face = static_cast<FT_Face>(m_face);

        // pitch = number of bytes for each row, pixels are always row major
        // however, the flow of the image (Y origin) can be top or down.
        // if pitch > 0, the 'flow' is to down, origin=up => We need to reverse Y
        // if pitch < 0, the 'flow' is to up, origin=down => How OpenGL treats texture (Textures origin is bottom-left corner)

        glm::vec2 bearing;
        bearing.x = face->glyph->bitmap_left;
        bearing.y = face->glyph->bitmap_top;

        // The FT_Glyph advance is in 1/64 pixel but Glyph advance is in pixel
        float advance = static_cast<float>(face->glyph->advance.x) / 64.0f;

        // The final character that will be loaded for later use
        registerGlyph(face->glyph->bitmap, c, bearing, advance);
    }

    FaceOutline::FaceOutline(FT_Library library, const std::filesystem::path &path, int characterSize)
            : Face(library, path, characterSize),
              m_stroker(library)
    {
    }

    void FaceOutline::loadGlyph(char c)
    {
        FT_UInt glyphIndex = FT_Get_Char_Index(m_face, c);

        float widthPx = getLineHeight() / 64.0f; // stroke width in pixel, that is the thickness of the curve that form the letters
        // widthPx is a constant, so the thickness is not customizable
        // Otherwise a lot of texture would be needed
        // However, it can be scaled

        int miter_limit = 0.0; // 16.16; argument for FT_Stroker_Set
        FT_Stroker_LineJoin_ line_join = FT_STROKER_LINEJOIN_BEVEL; // http://tavmjong.free.fr/SVG/LINEJOIN_STUDY/
        FT_Stroker_LineCap_ line_cap = FT_STROKER_LINECAP_SQUARE;
        // https://stackoverflow.com/questions/20874056/draw-text-outline-with-freetype
        FT_Stroker_Set(m_stroker, static_cast<int>(widthPx * 64.0f), line_cap, line_join, miter_limit);
        // radius is same unit as outline coords, that is 26.6 fixed-point

        FT_Check(FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_DEFAULT));

        FT_Glyph glyph;

        FT_Check(FT_Get_Glyph(static_cast<FT_Face>(m_face)->glyph, &glyph));
        FT_Check(FT_Glyph_Stroke(&glyph, m_stroker, false));
        FT_Check(FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true));
        auto bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

        glm::vec2 bearing = {bitmapGlyph->left, bitmapGlyph->top};
        float advance = glyph->advance.x / 65536.0f; // 16.16. For some reasons, not 26.6 like in FT_GlyphSlot

        registerGlyph(bitmapGlyph->bitmap, c, bearing, advance);

        FT_Done_Glyph(glyph);
    }
}