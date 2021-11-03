#include "Sprite.hpp"
#include <snk/Rect.hpp>

Sprite::Sprite(const glm::vec2& size)
    : m_size(size)
{
    Rect posRect(m_size);
    Rect texRect({1.0f, 1.0f}); // uv is always in [0;1]

    Vertex bottomRight;
    bottomRight.pos = posRect.bottomRight();
    bottomRight.uv = texRect.bottomRight();

    Vertex topRight;
    topRight.pos = posRect.topRight();
    topRight.uv = texRect.topRight();

    Vertex topLeft;
    topLeft.pos = posRect.topLeft();
    topLeft.uv = texRect.topLeft();

    Vertex bottomLeft;
    bottomLeft.pos = posRect.bottomLeft();
    bottomLeft.uv = texRect.bottomLeft();

    m_vertices[0] = bottomRight;
    m_vertices[1] = topRight;
    m_vertices[2] = topLeft;
    m_vertices[3] = bottomLeft;
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
