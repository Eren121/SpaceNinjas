#pragma once

#include "Shape.hpp"
#include <array>

/// @brief A sprite is simply a rectangular shape.
/// @details bottom-left == (0, 0); top-right == (1, 1)
class Sprite : public Shape
{
public:
    Sprite();
    explicit Sprite(const glm::vec2& size);

    size_t getVerticesCount() const override;
    Vertex getVertex(int index) const override;

    void setCoords(const Rect& rect);

    /// @brief Set the rect of the texture in normalized unit between [0;1].
    void setUVs(const Rect& rect);

    enum Layout {
        Zero,
        Centered
    };
    
    using Shape::setOrigin;
    void setOrigin(Layout x, Layout y);
    void setOrigin(Layout xy);

    /// @brief Get all corners in an array, in CCW-order with origin bottom-right.
    template<typename T>
    static std::array<glm::tvec2<T>, 4> asArray(const tRect<T>& rect)
    {
        return {rect.bottomRight(), rect.topRight(), rect.topLeft(), rect.bottomLeft()};
    }

private:
    glm::vec2 m_size{1.0f};
    Vertex m_vertices[4];
};