#pragma once

#include "wrappers/gl/Sprite.hpp"

namespace SpaceNinja
{
    /// @brief User data for fixture, contains mostly for graphics rendering
    struct DataFixture
    {
        /// @brief If not null, contains the graphical representation of this fixture
        /// @details The drawable should have all its vertices in coordinate system of the body (same as the fixtures)
        std::shared_ptr<Sprite> sprite;


        /// @brief Saved texture (data used for debugging).
        const Texture *savedTexture;
    };
}