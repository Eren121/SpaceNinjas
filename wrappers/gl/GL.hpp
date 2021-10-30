#pragma once

#include <utility/offset_of.hpp>
#include "OpenGL.hpp"
#include <vector>
#include <utility>

/// @brief Macro to expand vectors, since some OpenGL functions take consecutive parameters instead of one vector.
#define glVec2(v) (v).x, (v).y
#define glVec3(v) (v).x, (v).y, (v).z
#define glVec4(v) (v).x, (v).y, (v).z, (v).w

/// @brief Utility classes, functions and RAII for OpenGL objects.
namespace GL
{
    /// @brief RAII OpenGL object
    template<typename T = unsigned int, T invalid_value = T(0)>
    struct GLObject
    {
        using value_type = T;

        GLObject() = default;
        virtual ~GLObject() = default;

        GLObject& operator=(GLObject&& rhs) noexcept
        {
            swap(*this, rhs);
            return *this;
        }

        explicit GLObject(GLObject&& rhs)
        {
            swap(*this, rhs);
        }

        GLObject& operator=(const GLObject&) = delete;
        GLObject(const GLObject&) = delete;

        /// @brief Non-explicit because opengl ID are normally really only integers.
        operator value_type() const { return id; }

        value_type id{invalid_value};

        /// @remarks We have to define this function otherwise move constructors will go into infinite recursion
        friend void swap(GLObject& a, GLObject& b)
        {
            std::swap(a.id, b.id);
        }
    };


    struct Shader : GLObject<>
    {
        Shader(GLenum type);
        ~Shader();

        Shader(Shader&&) = default;
        Shader& operator=(Shader&&) = default;
    };

    struct Program : GLObject<>
    {
        Program();
        ~Program();

        Program(Program&&) = default;
        Program& operator=(Program&&) = default;
    };

    struct Buffer : GLObject<>
    {
        Buffer();
        ~Buffer() override;

        Buffer(Buffer&&) = default;
        Buffer& operator=(Buffer&&) = default;
    };

    struct Texture : GLObject<>
    {
        Texture();
        ~Texture() override;

        Texture(Texture&&) = default;
        Texture& operator=(Texture&&) = default;
    };


    struct VertexArray : GLObject<>
    {
        VertexArray();
        ~VertexArray();

        VertexArray(VertexArray&&) = default;
        VertexArray& operator=(VertexArray&&) = default;
    };

    /// @brief Catch all errors and print them to stderr when there is one.
    /// @see https://www.khronos.org/opengl/wiki/OpenGL_Error
    void enableDebugging(bool throwOnError = true);

    template<typename T>
    void bufferData(GLenum target, const std::vector<T>& buffer, GLenum usage)
    {
        glBufferData(target, buffer.size() * sizeof(buffer[0]), buffer.data(), usage);
    }

    /// @brief Enable and setup with glVertexAttribPointer()
    /// @details It can be tricky in C++ to use stride, because members are not necessarily packed.
    template<typename Class, typename FieldType>
    void vertexAttribPointer(GLuint index, GLint size, GLenum type, FieldType(Class::*field))
    {
        // Compute the offset of the member
        // offset is C and doesn't works with templates
        // Taking the address will not dereference, not causing segfault.

        glVertexAttribPointer(index, size, type, GL_FALSE, sizeof(Class), reinterpret_cast<const void*>(offset_of(field)));
        glEnableVertexAttribArray(index);
    }
};

