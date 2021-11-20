#pragma once

#include <gl/Sprite.hpp>
#include <box2d/b2_fixture.h>

namespace sn
{
    /// @brief User data for fixture, contains mostly for graphics rendering
    struct DataFixture
    {
        /// @brief If not null, contains the graphical representation of this fixture
        /// @details The drawable should have all its vertices in coordinate system of the body (same as the fixtures)
        std::unique_ptr<Sprite> sprite;

        /// @brief Saved texture (data used for debugging).
        const Texture *savedTexture{nullptr};
    };

    DataFixture& getData(b2Fixture& fixture);
    const DataFixture& getData(const b2Fixture& fixture);
}