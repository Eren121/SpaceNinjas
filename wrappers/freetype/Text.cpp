#include "Text.hpp"
#include "private/Face.hpp"
#include <glm/gtc/matrix_transform.hpp>

Text::Text()
{
}

void Text::setFont(const Font *font)
{
    m_font = font;
    onChanged();
}

const Font* Text::getFont() const
{
    return m_font;
}

void Text::setString(const std::string& string)
{
    m_string = string;
    onChanged();
}

const std::string& Text::getString() const
{
    return m_string;
}

void Text::setColor(const glm::vec4& color)
{
    m_color = color;
    onChanged();
}

const glm::vec4& Text::getColor() const
{
    return m_color;
}

glm::vec2 Text::getSize() const
{
    ensureUpdated();
    return m_size;
}

void Text::update() const
{
    m_size = {0.0f, 0.0f}; // Bounding box of the characters (use line and not glyph textures as bounds)
    m_segments.clear();

    if(m_font && !m_string.empty())
    {
        FontType type{m_characterSize, false};
        const FT::Face& face = m_font->getFace(type);

        const float height = face.getLineHeight();

        // Origin of each character in Text space
        // We set the origin of text to be the line above
        // Text origin is top-left
        glm::vec2 cursor {0.0f};
        cursor.y -= face.getAscender();

        VertexArray *curSegment = nullptr;


        auto append = [&](const Glyph& glyph, const glm::vec4& color) {

            // Add a new segment if either:
            //  1) this is the first glyph
            //  2) the new glyph belongs to a face different than the previous glyph
            //  3) the new glyph belongs to the same face as the previous glyph, but on a different page
            //  Points 2) and 3) are treated the same, by checking if their respective textures points on the same texture.

            if(curSegment == nullptr || glyph.texture != curSegment->getTexture())
            {
                curSegment = &m_segments.emplace_back();
                curSegment->setPrimitiveType(GL_TRIANGLES);
                curSegment->setTexture(glyph.texture);
            }

            blitChar(*curSegment, glyph, cursor, color);
        };

        for (char c : m_string)
        {
            if(c == '\n')
            {
                // Treats the character '\n' separately
                // Next glyphs will be drawn on the next line
                cursor.y -= height;
                cursor.x = 0;
            }
            else
            {
                // Add the Glyph if it's not a newline (invisible character)
                const Glyph& glyph = face.getGlyph(c);
                append(glyph, m_color);

                if(m_hasOutline)
                {
                    FontType outlineType{m_characterSize, true};
                    const Glyph& outline_glyph = m_font->getFace(outlineType).getGlyph(c);
                    append(outline_glyph, m_outlineColor);
                }

                cursor.x += glyph.advance;
            }

            m_size.x = std::max(m_size.x, cursor.x);
        }

        // cursor.y is negative, we want the size to be absolute
        // descender is also negative
        // So we take the opposite
        // Also, we don't add ascender because ascender is included in cursor.y
        m_size.y = -(cursor.y + face.getDescender());
    }
}

void Text::draw(RenderStates states) const
{
    ensureUpdated();

    if(states.shader)
    {
        states.model *= getTransform();

        for(const VertexArray &segment : m_segments)
        {
            segment.draw(states);
        }
    }
}

void Text::drawBoundingBox(RenderStates states) const
{
    ensureUpdated();

    states.model *= getTransform();

    Sprite sprite;
    sprite.setOrigin({0.0f, 1.0f});
    sprite.setColor({0.0f, 1.0f, 0.0f, 0.5f});
    sprite.setScale(getSize());

    sprite.draw(states);
}

void Text::setOutlineColor(const glm::vec4 &color)
{
    m_outlineColor = color;
    onChanged();
}

const glm::vec4& Text::getOutlineColor() const
{
    return m_outlineColor;
}

void Text::setOutline(bool has)
{
    m_hasOutline = has;
    onChanged();
}

bool Text::hasOutline() const
{
    return m_hasOutline;
}

void Text::blitChar(VertexArray& varr, const Glyph& glyph, const glm::vec2& cursor, const glm::vec4& color) const
{
    Rect pos;
    pos.origin = cursor + glyph.bearing - glm::vec2(0, glyph.size.y); // Origin is bottom-left
    pos.size = glyph.size;

    addRect(varr, pos, glyph.uv, color);
}

void Text::addRect(VertexArray& varr, const Rect& pos, const Rect& uvs, const glm::vec4& color)
{
    for(auto corner : {&Rect::bottomLeft,  &Rect::bottomRight, &Rect::topLeft,
                       &Rect::bottomRight, &Rect::topRight,    &Rect::topLeft})
    {
        Vertex v;
        v.pos = (pos.*corner)();
        v.uv = (uvs.*corner)();
        v.color = color;
        varr.addVertex(v);
    }
}

void Text::setOpacity(float opacity)
{
    m_color.a = opacity;
    m_outlineColor.a = opacity;
    onChanged();
}

void Text::setCharacterSize(int characterSize)
{
    m_characterSize = characterSize;
    onChanged();
}

int Text::getCharacterSize() const
{
    return m_characterSize;
}

float Text::getLineHeight() const
{
    return m_font->getFace({m_characterSize, false}).getLineHeight();
}

void Text::setOrigin(Layout x, Layout y)
{
    ensureUpdated();
    
    // center.y is negative because Text origin is top-left
    
    const Layout layouts[] {x, y};
    const glm::vec2 center {m_size.x / 2.0f, -m_size.y / 2.0f};
    glm::vec2 orig;
    
    for(int i = 0; i < 2; ++i)
    {
        switch(layouts[i])
        {
            case Centered:
                orig[i] = center[i];
                break;
            
            case Zero:
                orig[i] = 0;
                break;
        }
    }
    
    setOrigin(orig);
}

void Text::setOrigin(Text::Layout xy)
{
    setOrigin(xy, xy);
}
