#include "b2/World.hpp"

namespace b2
{
    const Time World::DEFAULT_DELTA = Time::seconds(1.0f / 60.0f);

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

void World::step()
{
    m_simulationTime += m_delta;

    // The valus are for internal box2D engines; tutorial suggest 6 and 2
    const int velocityIterations = 6;
    const int positionIterations = 2;

    // here: https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_hello.html
    this->Step(m_delta.asSeconds(), velocityIterations, positionIterations);

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

long World::getIteration() const
{
    return m_iteration;
}

void World::cleanupDestroyedBodies(bool callDestroyCallback)
{
    assert(IsLocked() == false);

    for (auto *body : m_toDestroy)
    {
        onDestroy(*body);

        // Remove effectively the body from the world
        DestroyBody(body);
    }

    m_toDestroy.clear();
}

b2Body& World::createBody(b2BodyDef &def)
{
    b2Body &body = *CreateBody(&def);
    onCreate(body);
    return body;
}

}