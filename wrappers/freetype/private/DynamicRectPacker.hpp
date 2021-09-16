#pragma once

#include "utility/binpacking/MaxRectsBinPack.h"
#include "utility/Rect.hpp"
#include <map>

/// @brief Pack textures into one.
/// @details The size of the texture is dynamic. When there is not enough space, the size is doubled.
/// It does not manipulate OpenGL, only the rects.
/// Use the Bin Packer implementation: https://github.com/juj/RectangleBinPack by Jukka Jylnki.
class DynamicRectPacker
{
public:
    /// @brief Constructor.
    /// @param initialSize The initial size of the texture.
    explicit DynamicRectPacker(glm::vec2 initialSize = {256, 256});

    /// @return The ID of the inserted Rect
    /// @param doubled if not null, set to true if the size was doubled, false otherwise.
    int insert(glm::vec2 size, bool *doubled = nullptr);

    /// @brief Search a Rect from its ID. The id should exist.
    const Rect& operator[](int id) const;

private:
    rbp::MaxRectsBinPack& getPacker(glm::ivec2 mag);
    bool packerExists(glm::ivec2 mag) const;

    /// @returns true of the size was doubled
    bool goToNextMagnitude();

    void doubleMagnitude();

    glm::ivec2 m_initialSize;

    /// @brief Map from ID (starting at zero, grow with each inserted Rect) to the Texture coordinates.
    std::map<int, Rect> m_dictionary;

    /// @brief Next Rect ID. Incremented for each new Rect.
    int m_nextID {1};

    /// @brief Last packer coordinate that is not full.
    /// @details Increment X first, then Y.
    glm::ivec2 m_currentMag;

    /// @brief How many packers there is in each dimension of the Texture.
    int m_magnitude {1};

    /// @brief 2D map for each part of the Texture
    /// @details m_packers[x][y] give the packer at coordinates (x, y).
    std::map<int, std::map<int, rbp::MaxRectsBinPack>> m_packers;
};