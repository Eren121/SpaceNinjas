#pragma once

#include "Shape.hpp"

/// @brief A sprite is simply a rectangular shape.
/// @details bottom-left == (0, 0); top-right == (1, 1)
class Sprite : public Shape
{
public:
    explicit Sprite(const glm::vec2& size = {1.0f, 1.0f});

    size_t getVerticesCount() const override;
    Vertex getVertex(int index) const override;

    enum Layout {
        Zero,
        Centered
    };
    
    using Shape::setOrigin;
    void setOrigin(Layout x, Layout y);
    void setOrigin(Layout xy);
    
private:
    glm::vec2 m_size{1.0f};
    Vertex m_vertices[4];
};

