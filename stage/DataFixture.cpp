#include "DataFixture.hpp"

namespace SpaceNinja
{
    DataFixture& getData(b2Fixture& fixture)
    {
        return *static_cast<DataFixture*>(fixture.GetUserData().userData.get());
    }

    const DataFixture& getData(const b2Fixture& fixture)
    {
        // Box2D don't provide const getter...
        return *static_cast<DataFixture*>(const_cast<b2Fixture&>(fixture).GetUserData().userData.get());
    }
}

namespace b2
{
    std::function<void(FixtureUserData&)> FixtureUserData::construct = [](FixtureUserData& fud) {
        fud.userData = std::make_shared<SpaceNinja::DataFixture>();
    };
}