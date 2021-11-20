#include "gl/Sprite.hpp"
#include <Rect.hpp>

Sprite::Sprite()
    : Sprite{{1.0f, 1.0f}}
{
}

Sprite::Sprite(const glm::vec2& size)
    : m_size(size)
{
    const Rect posRect(m_size);
    const Rect texRect({1.0f, 1.0f}); // uv is always in [0;1]

    setCoords(posRect);
    setUVs(texRect);
}

size_t Sprite::getVerticesCount() const
{
    return 4;
}

Vertex Sprite::getVertex(int index) const
{
    return m_vertices[index];
}

void Sprite::setOrigin(Sprite::Layout x, Sprite::Layout y)
{
    const Layout layouts[] {x, y};
    const glm::vec2 center {m_size.x / 2.0f, m_size.y / 2.0f};
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

void Sprite::setOrigin(Sprite::Layout xy)
{
    setOrigin(xy, xy);
}

void Sprite::setCoords(const Rect& rect)
{
    int i = 0;
    for(const glm::vec2& pos : asArray(rect))
    {
        m_vertices[i].pos = pos;
        ++i;
    }

    needUpdate();
}

void Sprite::setUVs(const Rect& rect)
{
    int i = 0;
    for(const glm::vec2& uv : asArray(rect))
    {
        m_vertices[i].uv = uv;
        ++i;
    }

    needUpdate();
}
