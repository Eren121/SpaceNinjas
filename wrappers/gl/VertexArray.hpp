#pragma once

#include "Vertex.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Transformable.hpp"
#include "wrappers/gl/GL.hpp"
#include "utility/LazyResource.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <type_traits>

static_assert(std::is_standard_layout_v<Vertex>, "Vertex should be standard layout");

class VertexArray : public Drawable, public Transformable, private LazyResource
{
public:
    VertexArray() = default;

    void setTexture(const Texture *texture);
    const Texture* getTexture() const;

    /// @brief Parameter usage to glBufferData(). Default's to GL_STATIC_DRAW
    /// @see https://www.khronos.org/registry/OpenGL-Refpages/es1.1/xhtml/glBufferData.xml
    /// @remarks Resent all the vertices to th GPU.
    void setUsage(GLenum usage);

    /// @brief Each attribute should have it's location as it (type corresponding in Vertex class)
    enum Attribute {
        POSITION = 0,
        COLOR = 1,
        UV = 2,
    };

    /// @brief Replace all vertices by @p vertices
    void setVertices(const std::vector<Vertex>& vertices);

    /// @brief Append a vertex to existing vertices
    void addVertex(const Vertex& vertex);

    /// @param type Should be one of https://www.khronos.org/opengl/wiki/Primitive (GL_TRIANGLES, GL_LINES, etc...)
    void setPrimitiveType(GLenum type);

    void draw(RenderStates states) const override;
protected:
    void update() const override;

private:
    GLenum m_primitive {GL_LINES};
    GLenum m_usage {GL_STATIC_DRAW};
    const Texture *m_texture {nullptr};

    /// @name LazyResource
    /// @{
    mutable bool m_needUpdate {true};
    mutable int m_verticesCount {0};
    mutable std::vector<Vertex> m_vertices; ///< @remarks Cleared when updated
    mutable GL::VertexArray m_vao;
    mutable GL::Buffer m_vbo;
    /// @}
};

