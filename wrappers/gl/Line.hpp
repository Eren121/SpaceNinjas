#pragma once

#include "VertexArray.hpp"

class Line : public VertexArray
{
public:
    Line() = default;

    void update(const glm::vec2& a, const glm::vec2& b, const glm::vec4& color = glm::vec4(1.0f), float thickness = 1.0f);

protected:
    std::vector<Vertex> loadVertices() const;

private:
    glm::vec2 m_a {0.0f, 0.0f};
    glm::vec2 m_b {0.0f, 0.0f};
    glm::vec4 m_color {1.0f, 1.0f, 1.0f, 1.0f};
    float m_thickness {0.0f};
};

