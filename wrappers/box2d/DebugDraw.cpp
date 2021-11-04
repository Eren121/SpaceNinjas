#include "DebugDraw.hpp"
#include <wrappers/box2d/box2d.hpp>
#include <snk/unused.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace b2;

DebugDraw::DebugDraw()
{
    // We use the same vertices for each shape, so we need to continuously change the vertices.
    m_shape.setUsage(GL_DYNAMIC_DRAW);
    m_circle.setDynamic(true);
    m_line.setUsage(GL_DYNAMIC_DRAW);
    
    
    this->SetFlags(~0); // All flags
}

void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color& color)
{
    std::vector<Vertex> vec;

    for(int i = 0; i < vertexCount; ++i)
    {
        Vertex v;
        v.color = b2::toGLM(color);
        v.pos = b2::toGLM(vertices[i]);

        vec.push_back(v);
    }

    m_shape.setVertices(vec);
    m_shape.setPrimitiveType(GL_LINE_LOOP);
    m_shape.draw(states);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color& queryColor)
{
    std::vector<Vertex> vec;

    b2Color color{queryColor};
    color.a *= 0.25;

    for(int i = 0; i < vertexCount; ++i)
    {
        Vertex v;
        v.color = b2::toGLM(color);
        v.pos = b2::toGLM(vertices[i]);

        vec.push_back(v);
    }

    m_shape.setVertices(vec);
    m_shape.setPrimitiveType(GL_TRIANGLE_FAN);
    m_shape.draw(states);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    RenderStates child = this->states;
    child.model = glm::translate(child.model, {center.x, center.y, 0.0f});
    child.model = glm::scale(child.model, glm::vec3{radius});

    {
        glm::vec4 shapeColor = b2::toGLM(color);
        shapeColor.a = 0.5f;

        m_circle.setColor(shapeColor);
        m_circle.draw(child);
    }
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& queryColor)
{
    b2Color color{queryColor};
    color.a *= 0.25;

    RenderStates child = this->states;
    child.model = glm::translate(child.model, {center.x, center.y, 0.0f});
    child.model = glm::scale(child.model, glm::vec3{radius});

    {
        glm::vec4 shapeColor = b2::toGLM(color);
        shapeColor.a = 0.5f;

        m_circle.setColor(shapeColor);
        m_circle.draw(child);
    }

    {
        // Draw axis
        // axis: vector from center to radius, representing angle 0°

        glm::vec4 shapeColor = 1.0f - b2::toGLM(color);
        shapeColor.a = 0.75f;

        glm::vec2 a {0.0f, 0.0f};
        glm::vec2 b = b2::toGLM(axis);

        m_line.update(a, b, shapeColor);
        m_line.draw(child);
    }
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    glm::vec4 shapeColor = b2::toGLM(color);
    shapeColor.a = 0.5f;

    glm::vec2 a = b2::toGLM(p1);
    glm::vec2 b = b2::toGLM(p2);

    m_line.update(a, b, shapeColor);
    m_line.draw(states);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
    UNUSED(xf);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
    DrawCircle(p, size, color);
}