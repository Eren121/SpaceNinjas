#pragma once

#include "utility/list_iterator.hpp"
#include "BodyType.hpp"
#include <entt/entt.hpp>

// Since this file is included in b2_user_settings.h,
// we can't include any Box2D file to prevent circular inclusion.
class b2Body;
class b2Fixture;

namespace SpaceNinja
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
    public:
        /// @details
        ///     Never change, a entt::const_handle means that the referred entity is always the same.
        entt::handle handle;

        explicit DataBody() = default;
        DataBody(const entt::handle& handle, b2Body& body, BodyType type);

        b2Body& getBody() { return *m_body; }
        const b2Body& getBody() const { return *m_body; }

        list_iterator<b2Fixture, next> begin() const;
        list_iterator<b2Fixture, next> end() const;

        BodyType type{BodyType::None};

        operator bool() const { return m_body; }

    private:

        /// @details
        ///     Box2D never invalidate their pointers. We don't use a reference to be copiable.
        ///     So we keep the pointer private to never change and expose a public getter.
        b2Body *m_body{nullptr};
    };
}