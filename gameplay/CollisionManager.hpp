#pragma once

#include "Body.hpp"
#include <wrappers/box2d/World.hpp>
#include <map>
#include <sigslot/signal.hpp>

/// @brief Permit to add callback when two types of objects collides
class CollisionManager : public b2ContactFilter, public sigslot::observer
{
public:
    explicit CollisionManager(b2::World& world);
    /// @brief Register a callback when two objects collide
    /// @details The order of the arguments doesn't matter.
    /// @param callback A callable that will be called when a collision occurs with a signature void(bodyA&, bodyB&).
    template<typename T>
    sigslot::connection connect(Body::Type a, Body::Type b, T&& callback);

    /// @brief Register a callback when an object collide with any other object.
    template<typename T>
    sigslot::connection connect(Body::Type a, T&& callback);

    /// @brief Disable or enable totally collision between two types
    /// @details By default, all bodies can collide.
    /// Will only work if the CollisionManager is set as the contact filter of the world.
    void setCollisionEnabled(Body::Type a, Body::Type b, bool canCollide);

private:
    bool ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB) override;

    void onCollide(b2Body& a, b2Body& b);

    /// @brief In the pair, the lowest integer type should be always first.
    using TypePair = std::pair<Body::Type, Body::Type>;

    /// @brief Make a pair of types
    /// @param swapped if not null, will contains true if the types were swapped because the lowest type is always first.
    static TypePair make_pair(Body::Type a, Body::Type b, bool *swapped = nullptr);

    /// @brief
    /// Key: <Type A, Type B>
    /// Value: the callback to call signal(bodyA, bodyB)
    std::map<TypePair, sigslot::signal<b2Body&, b2Body&>> m_callbacks;

    /// @brief Callback for any type
    /// Key: <Type A>
    /// Value: the callback to call signal(bodyA, anyBody)
    std::map<Body::Type, sigslot::signal<b2Body&, b2Body&>> m_anyCallbacks;

    /// @brief Set for the bodies types that can't collide.
    std::set<TypePair> m_cannotCollide;
};

template<typename T>
sigslot::connection CollisionManager::connect(Body::Type a, Body::Type b, T&& callback)
{
    // Since the collision order ((A, B) or (B, A)) is undefined and we want to always catch the collision
    // independently from the order, a simple rule is used:
    // in the callback table, the lower integer type is always first.

    if(a <= b)
    {
        return m_callbacks[{a, b}].template connect(callback);
    }
    else
    {
        // Do not forget to also reverse the callback arguments!!!
        return m_callbacks[{b, a}].template connect([cb = std::forward<T>(callback)](b2Body& b, b2Body& a) {
            cb(a, b);
        });
    }
}

template<typename T>
sigslot::connection CollisionManager::connect(Body::Type a, T&& callback)
{
    return m_anyCallbacks[a].template connect(callback);
}
