#pragma once

#include "gameplay/Body.hpp"
#include "wrappers/box2d/World.hpp"
#include <map>
#include <sigslot/signal.hpp>

/// @brief Permit to add callback when two types of objects collides
class CollisionManager
    : public b2ContactFilter,
      public b2ContactListener,
      public sigslot::observer
{
public:
    enum EventType {
        Begin,
        End
    };

    /// @brief Register a callback when two objects collide
    /// @details The order of the arguments doesn't matter.
    /// @param callback A callable that will be called when a collision occurs with a signature void(bodyA&, bodyB&).
    /// @param type What specific event to listen to.
    template<typename T>
    sigslot::connection onEvent(Body::Type a, Body::Type b, EventType type, T callback);

    template<typename T>
    sigslot::connection onEvent(Body::Type a, EventType type, T callback);

    /// @brief Disable or enable totally collision between two types
    /// @details By default, all bodies can collide.
    /// Will only work if the CollisionManager is set as the contact filter of the world.
    void setCollisionEnabled(Body::Type a, Body::Type b, bool canCollide);

private:
    bool ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB) override;
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

    void onEvent(b2Body& a, b2Body& b, EventType type);

    struct TypePair
    {
        /// @param swapped if not null, will contains true if the types were swapped because the lowest type is always first.
        explicit TypePair(Body::Type typeA, Body::Type typeB, bool *swapped = nullptr);

        Body::Type typeA;
        Body::Type typeB;

        auto operator<=>(const TypePair&) const = default;
    };

    struct EventKey : TypePair
    {
        explicit EventKey(Body::Type typeA, Body::Type typeB, EventType type, bool *swapped = nullptr)
            : TypePair(typeA, typeB, swapped), type(type) {}

        EventType type;

        auto operator<=>(const EventKey&) const = default;
    };

    /// @brief Callbacks when a collision (enter or exit) happens with 2 exact types.
    std::map<EventKey, sigslot::signal<b2Body&, b2Body&>> m_exactCallbacks;

    /// @brief Callbacks when a collision (enter or exit) happens with one exact type and the other one can be any type.
    /// @details key.typeB should be Body::None.
    std::map<EventKey, sigslot::signal<b2Body&, b2Body&>> m_anyCallbacks;

    /// @brief Set for the bodies types that can't collide.
    std::set<TypePair> m_cannotCollide;
};

template<typename T>
sigslot::connection CollisionManager::onEvent(Body::Type a,
                                              Body::Type b,
                                              CollisionManager::EventType type,
                                              T callback)
{
    // Since the collision order ((A, B) or (B, A)) is undefined and we want to always catch the collision
    // independently from the order, a simple rule is used:
    // in the callback table, the lower integer type is always first.

    bool swapped{};
    const EventKey key{a, b, type, &swapped};

    if(swapped)
    {
        return m_exactCallbacks[key].connect([cb = std::forward<T>(callback)](b2Body& b, b2Body& a) {
            // Do not forget to also reverse the callback arguments!!!
            cb(a, b);
        });
    }
    else
    {
        return m_exactCallbacks[key].connect(std::forward<T>(callback));
    }
}
template<typename T>
sigslot::connection CollisionManager::onEvent(Body::Type a,
                                              CollisionManager::EventType type,
                                              T callback)
{
    // Since the collision order ((A, B) or (B, A)) is undefined and we want to always catch the collision
    // independently from the order, a simple rule is used:
    // in the callback table, the lower integer type is always first.

    const EventKey key{a, Body::Type::None, type};
    return m_anyCallbacks[key].connect(std::forward<T>(callback));
}