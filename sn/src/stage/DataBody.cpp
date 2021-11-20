#include "stage/DataBody.hpp"
#include "stage/StageWorld.hpp"
#include <b2/b2.hpp>
#include <list_iterator.hpp>

namespace sn
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

    void DataBody::setSensor(bool sensor)
    {
        for(b2Fixture& fixture : *m_body)
        {
            fixture.SetSensor(sensor);
        }
    }

    snk::list_iterator<b2Fixture, next> DataBody::begin() const
    {
        return m_body->GetFixtureList();
    }

    snk::list_iterator<b2Fixture, next> DataBody::end() const
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

    StageWorld& DataBody::getWorld() { return static_cast<StageWorld&>(*m_body->GetWorld()); }

    DataBody& getData(b2Body& body)
    {
        return *static_cast<DataBody*>(body.GetUserData().userData.get());
    }

    const DataBody& getData(const b2Body& body)
    {
        // Box2D bug: GetUserData has no non-const getter
        return *static_cast<DataBody*>(const_cast<b2Body&>(body).GetUserData().userData.get());
    }

    void DataBody::destroy()
    {
        if(isValid())
        {
            getWorld().markForDestroy(*this);
        }
        else
        {
            getLogger().warn("Tried to destroy an invalid entity");
        }
    }
}

namespace b2
{
    std::function<void(BodyUserData&)> BodyUserData::construct = [](BodyUserData& bud) {
        bud.userData = std::make_shared<sn::DataBody>();
    };
}