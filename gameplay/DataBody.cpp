#include "DataBody.hpp"
#include <box2d/box2d.h>

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
}
