#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_draw.h>
#include <glm/glm.hpp>

/// @brief box2d utility functions
namespace b2
{
inline glm::vec2 toGLM(const b2Vec2& v)
{
    return {v.x, v.y};
}

inline glm::vec4 toGLM(const b2Color& c)
{
    return {c.r, c.g, c.b, c.a};
}

inline b2Vec2 fromGLM(const glm::vec2& v)
{
    return {v.x, v.y};
}

/// @name
/// @brief Box2D provide only SetTransform for both position and angle, but often we need to change only one
/// @{
inline void setPosition(b2Body& body, const glm::vec2& newPos)
{
    body.SetTransform(b2::fromGLM(newPos), body.GetAngle());
}

inline glm::vec2 getPosition(b2Body& body)
{
    return toGLM(body.GetPosition());
}

inline void setAngle(b2Body& body, float newAngle)
{
    body.SetTransform(body.GetPosition(), newAngle);
}

/// @}

}