#pragma once

#include <snk/list_iterator.hpp>
#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>

namespace b2
{
    using fixture_next_mem = b2Fixture*(b2Fixture::*)();
    using body_iterator = snk::list_iterator<b2Fixture, static_cast<fixture_next_mem>(&b2Fixture::GetNext)>;
}

inline b2::body_iterator begin(b2Body &body) { return body.GetFixtureList(); }
inline b2::body_iterator end(b2Body&) { return nullptr; }