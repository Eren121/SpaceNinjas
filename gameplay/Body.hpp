#pragma once

#include <wrappers/gl/Texture.hpp>
#include <wrappers/gl/Sprite.hpp>
#include <wrappers/box2d/Fwd.hpp>

/// @brief User data for b2::Body
/// @details Contains all data needed for rendering plus object data.
/// @remarks
/// For the rotation: if the body has an angle of 0°, then the texture will be rendered as-is, without rotation.
/// The right part of the texture will always be rendered as the part of the Sprite that is at it's angle.
class Body : public Sprite
{
public:
    enum Type {
        None,
        Player,
        Ennemy,
        PlayerLimits, ///< Barriers at the limit of the world, to garbage collect entities colliding with it
        Universe, ///< Everything that end collision with this body is destroyed

        PlayerMissile,

        Count ///< Do not use, to know the count of Types
    } const type;


    /// @brief Create a body data from a body that should be a box.
    /// @param rx,ry Same parameters as b2PolygonShape::SetAsBox().
    Body(Type type, b2Body& body, float rx, float ry);
    Body(Type type, b2Body& body, const glm::vec2& radius);

    void draw(RenderStates states) const;

private:
    b2Body& m_body;
};

