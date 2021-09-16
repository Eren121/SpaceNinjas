#include "VertexArray.hpp"

void VertexArray::setTexture(const Texture *texture)
{
    m_texture = texture;
}

const Texture* VertexArray::getTexture() const
{
    return m_texture;
}

void VertexArray::setVertices(const std::vector<Vertex>& vertices)
{
    m_vertices = vertices;
    onChanged();
}

void VertexArray::setPrimitiveType(GLenum type)
{
    m_primitive = type;
}

void VertexArray::draw(RenderStates states) const
{
    ensureUpdated();

    states.model *= getTransform();

    if(states.shader)
    {
        Shader::bind(states.shader);

        states.shader->setUniform("u_Color", glm::vec4{1.0f});
        states.shader->setUniform("u_ModelMatrix", states.model);
        states.shader->setUniform("u_ViewMatrix", states.view);
        states.shader->setUniform("u_ProjectionMatrix", states.projection);
    }

    Texture::bind(m_texture);

    glBindVertexArray(m_vao);
    glDrawArrays(m_primitive, 0, m_verticesCount);
}

void VertexArray::setUsage(GLenum usage)
{
    m_usage = usage;
    onChanged();
}

void VertexArray::addVertex(const Vertex &vertex)
{
    m_vertices.push_back(vertex);
    onChanged();
}

void VertexArray::update() const
{
    m_verticesCount = static_cast<int>(m_vertices.size());

    auto stride = static_cast<GLsizei>(sizeof(m_vertices[0]));
    auto nBytes = static_cast<GLsizeiptr>(m_vertices.size() * stride);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, nBytes, m_vertices.data(), m_usage);

    Vertex::vertexAttribPointer();

    m_vertices.clear();
}
