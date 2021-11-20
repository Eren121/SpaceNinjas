#pragma once

#include <logging.hpp>
#include <list_iterator.hpp>
#include <list_iterator.hpp>
#include "BodyType.hpp"
#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include "Fwd.hpp"

// Since this file is included in b2_user_settings.h,
// we can't include any Box2D file to prevent circular inclusion.
class b2Body;
class b2Fixture;

namespace sn
{
    /// @{
    /// @brief For DataBody iterator.
    b2Fixture* next(b2Fixture* fixture);
    /// @}

    /// @brief Act as user data for bodies.
    /// @details
    ///     Copiable, act like a pointer because the goal is to check if an entity is dead or not.
    ///     If the entity is dead, there is no way to use its data because the pointer is invalid.
    ///     So this class is copiable.
    ///     Also thin wrapper around b2Body.
    /// @remarks Iterable, iterate fixtures.
    class DataBody
    {
        LOGGABLE("DataBody")

    public:
        /// @details
        ///     Never change, a entt::const_handle means that the referred entity is always the same.
        entt::handle handle;

        DataBody() = default;
        DataBody(const entt::handle& handle, b2Body& body, BodyType type);

        b2Body& getBody() { return *m_body; }
        const b2Body& getBody() const { return *m_body; }

        StageWorld& getWorld();

        snk::list_iterator<b2Fixture, next> begin() const;
        snk::list_iterator<b2Fixture, next> end() const;

        BodyType type{BodyType::None};

        operator bool() const { return isValid(); }
        bool isValid() const { return handle.valid(); }

        glm::vec2 getPosition() const;
        void setPosition(const glm::vec2& pos);

        void setSensor(bool sensor);

        float getAngle() const;
        void setAngle(float);

        void setVelocity(const glm::vec2& vel);
        glm::vec2 getVelocity();

        /// @brief Set the velocity and update the angle to the direction.
        void setVelocityWithAngle(const glm::vec2& vel);

        /// @brief Wrapper for StageWorld::markForDestroy().
        void destroy();

    private:

        /// @details
        ///     Box2D never invalidate their pointers. We don't use a reference to be copiable.
        ///     So we keep the pointer private to never change and expose a public getter.
        b2Body *m_body{nullptr};
    };


    /// @brief Don't use config file because it needs box2D recompilation each time...
    DataBody& getData(b2Body& body);
    const DataBody& getData(const b2Body& body);
}