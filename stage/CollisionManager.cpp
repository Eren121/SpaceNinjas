#include "CollisionManager.hpp"
#include <box2d/b2_contact.h>
#include "DataBody.hpp"
#include "DataFixture.hpp"

namespace SpaceNinja
{
    CollisionManager::TypePair::TypePair(BodyType a, BodyType b, bool *swapped)
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

    void CollisionManager::setCollisionEnabled(BodyType a, BodyType b, bool canCollide)
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
        DataBody& userDataA = getData(a);
        DataBody& userDataB = getData(b);
        const BodyType typeA = userDataA.type;
        const BodyType typeB = userDataB.type;

        // Callbacks for any type
        {
            const EventKey key{typeA, BodyType::None, type};
            m_anyCallbacks[key](a, b);
        }
        {
            const EventKey key{typeB, BodyType::None, type};
            m_anyCallbacks[key](b, a);
        }

        // Callback for specific collision fo both a and b
        // Ignore if we can't get the type of both
        {
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
        const DataBody& userDataA = getData(*fixtureA->GetBody());
        const DataBody& userDataB = getData(*fixtureB->GetBody());
        const BodyType typeA = userDataA.type;
        const BodyType typeB = userDataB.type;

        const TypePair key{typeA, typeB};
        return !m_cannotCollide.contains(key);
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
}