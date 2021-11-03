#include "DataBody.hpp"
#include "wrappers/box2d/box2d.hpp"

namespace SpaceNinja
{
    b2Fixture* next(b2Fixture* fixture)
    {
        return fixture->GetNext();
    }

    DataBody::DataBody(const entt::handle& handle, b2Body &body, BodyType type)
        : handle(handle),
          type(type),
          m_body(&body)
    {
    }

    list_iterator<b2Fixture, next> DataBody::begin() const
    {
        return m_body->GetFixtureList();
    }

    list_iterator<b2Fixture, next> DataBody::end() const
    {
        return nullptr;
    }

    glm::vec2 DataBody::getPosition() const { return b2::getPosition(*m_body); }
    void DataBody::setPosition(const glm::vec2 &pos) { b2::setPosition(*m_body, pos); }
    float DataBody::getAngle() const { return b2::getAngle(*m_body); }
    void DataBody::setAngle(float angle) { b2::setAngle(*m_body, angle); }
    void DataBody::setVelocity(const glm::vec2 &vel) { b2::setVelocity(*m_body, vel); }
    glm::vec2 DataBody::getVelocity() { return b2::getVelocity(*m_body); }
    void DataBody::setVelocityWithAngle(const glm::vec2 &vel) { b2::setVelocityWithAngle(*m_body, vel); }
}
