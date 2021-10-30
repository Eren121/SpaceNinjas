#include "CollisionManager.hpp"
#include <box2d/b2_contact.h>

CollisionManager::TypePair::TypePair(Body::Type a, Body::Type b, bool *swapped)
    : typeA(a), typeB(b)
{
    bool swappedLocal = false;

    if(a > b)
    {
        std::swap(typeA, typeB);
        swappedLocal = true;
    }

    if(swapped)
    {
        *swapped = swappedLocal;
    }
}

void CollisionManager::setCollisionEnabled(Body::Type a, Body::Type b, bool canCollide)
{
    const TypePair pair{a, b};

    if(canCollide)
    {
        m_cannotCollide.erase(pair);
    }
    else
    {
        m_cannotCollide.insert(pair);
    }
}

void CollisionManager::onEvent(b2Body& a, b2Body& b, EventType type)
{
    Body *const userDataA = a.GetUserData();
    Body *const userDataB = b.GetUserData();

    // Callbacks for any type
    if(userDataA)
    {
        const Body::Type typeA = userDataA->type;
        const EventKey key{typeA, Body::None, type};
        m_anyCallbacks[key](a, b);
    }
    if(userDataB)
    {
        const Body::Type typeB = userDataB->type;
        const EventKey key{typeB, Body::None, type};
        m_anyCallbacks[key](b, a);
    }

    // Callback for specific collision fo both a and b
    // Ignore if we can't get the type of both
    if(userDataA && userDataB)
    {
        const Body::Type typeA = userDataA->type;
        const Body::Type typeB = userDataB->type;

        bool swapped{};
        const EventKey key{typeA, typeB, type, &swapped};

        if(swapped)
        {
            m_exactCallbacks[key](b, a);
        }
        else
        {
            m_exactCallbacks[key](a, b);
        }
    }
}

bool CollisionManager::ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB)
{
    const Body *const userA = fixtureA->GetBody()->GetUserData();
    const Body *const userB = fixtureB->GetBody()->GetUserData();

    if(userA && userB)
    {
        const Body::Type typeA = userA->type;
        const Body::Type typeB = userB->type;
        const TypePair key{typeA, typeB};
        return !m_cannotCollide.contains(key);
    }
    else
    {
        // No user data, default collision filtering
        return b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
    }
}

void CollisionManager::BeginContact(b2Contact *contact)
{
    const EventType type{Begin};
    b2Body& a{*contact->GetFixtureA()->GetBody()};
    b2Body& b{*contact->GetFixtureB()->GetBody()};
    onEvent(a, b, type);
}

void CollisionManager::EndContact(b2Contact *contact)
{
    const EventType type{End};
    b2Body& a{*contact->GetFixtureA()->GetBody()};
    b2Body& b{*contact->GetFixtureB()->GetBody()};
    onEvent(a, b, type);
}
