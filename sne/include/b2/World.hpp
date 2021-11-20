#pragma once

#include "b2.hpp"
#include <list_iterator.hpp>
#include <time/Time.hpp>
#include <Rect.hpp>
#include <generator.hpp>
#include <sigslot/signal.hpp>
#include <functional>
#include <set>
#include <cassert>

namespace b2
{

generator<b2Fixture*> iterateFixtures(b2Body *body);

/// @brief Box2D world (inherits b2World) but with QOL features.
/// @remarks Since the base class has no virtual destructor, this class shall not be deleted via a base class pointer.
class World : public b2World
{
private:
    /// @brief Bodies to destroy at the end of the step.
    std::set<b2Body*> m_toDestroy;

    /// @brief Time of the simulation. Can only advance in a fixed time step of dt() seconds.
    Time m_simulationTime;

    /// @brief Count of iteration of the simulation, advanced by one at each step
    long m_iteration;

    /// @brief Effectively destroy all the bodies marked for destroying. Should not be called when locked.
    void cleanupDestroyedBodies(bool callDestroyCallback);

public:
    explicit World(b2Vec2 gravity = {0.0f, 0.0f});
    virtual ~World();

    /// @brief Callback when a body is created.
    /// @details The callback is called just after the body has spawn in the world, at the same frame.

    /// @brief Get the count of iteration of the simulation, advanced by one at each step, starting at 0.
    [[nodiscard]] long getIteration() const;

    /// @returns The current time of the simulation in seconds.
    /// @remarks It can only be a multiple of dt.
    [[nodiscard]] Time getTime() const;

    /// @name Iterate bodies
    /// @brief Iterable interface to iterate the bodies of the world.
    /// @{
    using body_iterator = snk::list_iterator<b2Body, static_cast<b2Body* (b2Body::*)()>(&b2Body::GetNext)>;
    using const_body_iterator = snk::list_iterator<const b2Body, static_cast<const b2Body* (b2Body::*)() const>(&b2Body::GetNext)>;

    body_iterator begin();
    body_iterator end();
    const_body_iterator begin() const;
    const_body_iterator end() const;
    /// @}

    /// @brief Destroy a body.
    /// @details The difference between b2World::DestroyBody() and this method is that this method can be called even
    /// when the world is locked. Is the world is locked, then the body to destroy are added to a list of destroyed
    /// bodies. All the bodies of this list will be destroyed end the end of the step. If step() is never
    /// called, the bodies will never be deleted.
    /// @remarks if you use only the default b2World::Step() method, then the bodies will not be destroyed.
    /// @remarks Will check if the body is deleted twice in the same time step, but not between two different timesteps.
    void markForDestroy(b2Body *body);

    /// @brief Create a body
    /// @remarks Calls onCreate().
    b2Body& createBody(b2BodyDef &def);

    /// @brief Callback just before a body is destroyed
    virtual void onDestroy(b2Body& body) {}

    /// @brief Callback just after a body is created.
    virtual void onCreate(b2Body& body) {}

    /// @brief Advance the simulation.
    /// @details Performs a semi-fixed step. Always performs only one step. You have to do {} while loop it if you
    /// want a real time simulation, with as condition until the time of the simulation is lower than the current real time.
    /// @remarks Also destroy the bodies marked as destroyed at the end of the step.
    void step();

    static const Time DEFAULT_DELTA;

protected:
    /// @brief How much time to advance at each step.
    /// @details
    ///      Publicly settable.
    ///      Tutorial suggests to have a constant simulation timestep (dependant from framerate)
    ///      However, we may use what is called a "semi-fixed" timestep
    ///      See https://gafferongames.com/post/fix_your_timestep/ for more information, implemented here
    ///      However we may have a non-constant timestep but for debugging purposes only.
    ///      Time step of physics, in seconds
    /// @remarks protected so can be set if needed by child classes, but to prevent it it's not public.
    Time m_delta{DEFAULT_DELTA};
};

}