#pragma once

#include <wrappers/gl/RenderStates.hpp>
#include <wrappers/gl/VertexArray.hpp>
#include <wrappers/gl/Circle.hpp>
#include <wrappers/gl/Line.hpp>
#include <box2d/box2d.h>

namespace b2
{

/// @brief OpenGL implementation for b2Draw.
class DebugDraw : public b2Draw
{
public:
    DebugDraw();

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color& color) override;
    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color& color) override;
    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
    void DrawTransform(const b2Transform& xf) override;
    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

    mutable RenderStates states;

private:
    /// @name
    /// @brief Store shapes to not create it at each draw, even if the vertices are updated at each call
    /// @{
    VertexArray m_shape;
    Circle m_circle;
    Line m_line;
    /// @}
};

}