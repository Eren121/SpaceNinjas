#pragma once

#include "stage/DataBody.hpp"
#include "stage/DataFixture.hpp"

/// @file Add utility functions related to creation of body with shapes.

namespace SpaceNinja
{
    /// @brief Add a box fixture to a body
    /// @param data The body to add a box.
    /// @param pos The position of the box relative to the body
    /// @param width
    ///     The width of the body.
    ///     The height will be proportional with the size of the texture.
    ///     The body will be centered on its position.
    /// @details
    ///     It will add a drawable box fixture with a texture (stored in fixture user data).
    ///     So it is possible to have multiple fixtures with different textures as the data
    ///     is not stored in the body but directly in the fixture.
    DataFixture &addBoxToBody(DataBody &data, const glm::vec2 &pos, float width, const std::string &texture);

    /// @param rect The bounds of the box relative to the body
    DataFixture &addBoxToBody(DataBody &data, const Rect& rect, const Texture *texture = nullptr);

    DataBody &createEnemyBody(StageWorld &world, const glm::vec2 &pos);

    DataBody &createMissileBody(StageWorld &world, const glm::vec2 &pos, const DataBody& thrower);

    DataBody &createPlayerBody(StageWorld &world, const glm::vec2 &pos);
}