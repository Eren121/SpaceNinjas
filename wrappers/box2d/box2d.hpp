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

inline void setUserData(b2Body& body, void *userData)
{
    body.GetUserData().pointer = reinterpret_cast<uintptr_t>(userData);
}

/// @brief Get the user Data of a Body, casting to the appropriate type.
/// @details
///     It is stored as this:
///     Body --> UserData (b2BodyUserData) --> pointer
///     The pointer is an integral type, we use it to store a pointer to our UserData.
///     Each body contains an owning pointer, that means the BodyUserData needs to be destroyed when the body is destroyed.
/// @returns The user data of the body. Returns it as a reference, so we can also set it to "nullptr".
/// @remarks If the user data is null, the cast is not a problem. That's also why a pointer is returned, the user data may be null.
template<typename UserData = void>
UserData* & getUserData(b2Body& body)
{
    return reinterpret_cast<UserData* &>(body.GetUserData().pointer);
}

/// @brief No need to return the pointer as reference as it is const, so unmodifiable in any way.
template<typename UserData = void>
const UserData* getUserData(const b2Body& body)
{
    // Box2D does not provides const GetUserData()
    // But we still get it to a const variable so it's ok
    const auto& b2UserData = const_cast<b2Body&>(body).GetUserData();
    return reinterpret_cast<const UserData*>(b2UserData.pointer);
}

/// @}

}