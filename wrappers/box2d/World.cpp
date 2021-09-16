#include "World.hpp"
#include <box2d/box2d.h>

namespace b2
{

generator<b2Fixture*> iterateFixtures(b2Body *body)
{
    for (auto fix = body->GetFixtureList(); fix != nullptr; fix = fix->GetNext())
    {
        co_yield fix;
    }
}

World::World(b2Vec2 gravity)
    : b2World(gravity), m_iteration(0)
{
    SetContactListener(this);
}

World::~World()
{
    // Destroy bodies to delete user data
    for(auto &body : *this)
    {
        markForDestroy(&body);
    }

    // Don't call callbacks in destructor
    cleanupDestroyedBodies(false);
}

Time World::dt()
{
    return Time::seconds(1.0f / 180.0f);
}

void World::step()
{
    auto stepSize = dt();

    m_simulationTime += stepSize;

    // The valus are for internal box2D engines; tutorial suggest 6 and 2
    // here: https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_hello.html
    this->Step(stepSize.asSeconds(), 6, 2);

    // Cleanup destroyed bodies at the end of the step
    cleanupDestroyedBodies(true);

    ++m_iteration;
}

World::body_iterator World::begin()
{
    return body_iterator{GetBodyList()};
}

World::body_iterator World::end()
{
    return body_iterator{nullptr};
}

World::const_body_iterator World::begin() const
{
    return const_body_iterator(GetBodyList());
}

World::const_body_iterator World::end() const
{
    return const_body_iterator(nullptr);
}

void World::markForDestroy(b2Body *body)
{
    // If the body is destroyed twice in the same time step, this will safely do nothing
    // and the body will not be deleted twice
    m_toDestroy.insert(body);
}

Time World::getTime() const
{
    return m_simulationTime;
}

    [[maybe_unused]] int World::getIteration() const
{
    return m_iteration;
}

void World::cleanupDestroyedBodies(bool callDestroyCallback)
{
    assert(IsLocked() == false);

    for (auto *body : m_toDestroy)
    {
        if(callDestroyCallback)
        {
            onDestroy(*body);
        }

        // Deallocate user data if there is
        void* &userData = b2::getUserData(*body);
        if (userData && deleter)
        {
            deleter(userData);
        }

        userData = nullptr; // Safety

        // Remove effectively the body from the world
        DestroyBody(body);
    }

    m_toDestroy.clear();
}

b2Body* World::createBody(b2BodyDef *def, void *userData)
{
    auto body = CreateBody(def);
    getUserData(*body) = userData;

    return body;
}

void World::BeginContact(b2Contact *contact)
{
    onCollide(*contact->GetFixtureA()->GetBody(), *contact->GetFixtureB()->GetBody());
}

b2Body& World::createBoxBody(const Rect& rect, b2BodyType type, float density, bool bullet)
{
    b2BodyDef def;
    def.type = type;
    def.bullet = bullet;
    def.position = b2::fromGLM(rect.center());

    b2Body *body = CreateBody(&def);

    b2PolygonShape box;
    box.SetAsBox(rect.size.x / 2.0f, rect.size.y / 2.0f);

    body->CreateFixture(&box, density);

    return *body;
}

}