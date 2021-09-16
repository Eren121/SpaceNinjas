#include "CollisionManager.hpp"

CollisionManager::CollisionManager(b2::World& world)
{
    world.onCollide.connect(&CollisionManager::onCollide, this);
    world.SetContactFilter(this);
}

CollisionManager::TypePair CollisionManager::make_pair(Body::Type a, Body::Type b, bool *swapped)
{
    CollisionManager::TypePair ret;
    bool swappedLocal = false;

    if(a > b)
    {
        ret = {b, a};
        swappedLocal = true;
    }
    else
    {
        ret = {a, b};
    }

    if(swapped)
    {
        *swapped = swappedLocal;
    }

    return ret;
}

void CollisionManager::setCollisionEnabled(Body::Type a, Body::Type b, bool canCollide)
{
    TypePair pair = make_pair(a, b);

    if(canCollide)
    {
        m_cannotCollide.erase(pair);
    }
    else
    {
        m_cannotCollide.insert(pair);
    }
}

void CollisionManager::onCollide(b2Body& a, b2Body& b)
{
    auto userDataA = b2::getUserData<Body>(a);
    auto userDataB = b2::getUserData<Body>(b);

    // Callbacks for any type
    if(userDataA)
    {
        Body::Type typeA = userDataA->type;
        m_anyCallbacks[typeA](a, b);
    }
    if(userDataB)
    {
        Body::Type typeB = userDataB->type;
        m_anyCallbacks[typeB](b, a);
    }

    // Callback for specific collision fo both a and b
    // Ignore if we can't get the type of both
    if(userDataA && userDataB)
    {
        Body::Type typeA = userDataA->type;
        Body::Type typeB = userDataB->type;

        // Re-order is necessary (if typeA == typeB, it is also OK)
        if(typeA > typeB)
        {
            // Swap order
            m_callbacks[std::make_pair(typeB, typeA)](b, a);
        }
        else
        {
            m_callbacks[std::make_pair(typeA, typeB)](a, b);
        }
    }
}

bool CollisionManager::ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB)
{
    Body *userA = b2::getUserData<Body>(*fixtureA->GetBody());
    Body *userB = b2::getUserData<Body>(*fixtureB->GetBody());

    if(userA && userB)
    {
        Body::Type typeA = userA->type;
        Body::Type typeB = userB->type;
        return !m_cannotCollide.contains(make_pair(typeA, typeB));
    }
    else
    {
        // No user data, default collision filtering
        return b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
    }
}
