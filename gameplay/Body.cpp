#include "Body.hpp"
#include "wrappers/box2d/box2d.hpp"
#include <glm/gtx/transform.hpp>

Body::Body(Body::Type type, b2Body& body, const glm::vec2& h)
    : Body(type, body, h.x, h.y)
{
}

Body::Body(Type type, b2Body& body, float hx, float hy)
    : Sprite({hx * 2.0f, hy * 2.0f}), type(type), m_body(body)
{
    // hx,hy are radius (semi-diameter) so we need to * 2.

    setOrigin({hx, hy}); // centerize the sprite
}

void Body::draw(RenderStates states) const
{
    auto b2Pos = m_body.GetPosition();
    glm::vec3 pos(b2Pos.x, b2Pos.y, 0.0f);

    // Apply the body's transform to the model matrix

    // Move the model to the position of the body
    states.model = glm::translate(states.model, pos);

    // Rotate the model to the rotation of the body
    // OpenGL is right handed in clip space (visible only in range [-1;1])
    // +z -> towards you -> rotation in CCW order (box2D use radians)
    static const glm::vec3 axis = {0.0f, 0.0f, 1.0f};
    states.model = glm::rotate(states.model, m_body.GetAngle(), axis);
    Sprite::draw(states);
}
