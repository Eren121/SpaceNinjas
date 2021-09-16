#pragma once

#include "wrappers/gl/GL.hpp"
#include "utility/Rect.hpp"
#include <SDL2/SDL_surface.h>
#include <glm/vec2.hpp>
#include <filesystem>

class Texture
{
public:
    Texture();

    /// @brief Set the texture as 1x1 opaque white
    void load1x1White();

    /// @brief Load from an image file on the disk.
    void load(const std::filesystem::path& path);

    /// @brief Load from a SDL_Surface.
    /// @param surface The surface from which to load. Not const because the surface will save copy informations.
    void load(SDL_Surface *surface);

    /// @brief Load as texture of this size with an undefined color
    void load(glm::ivec2 size);

    /// @brief Load as texture of this size with an uniform color
    void load(glm::ivec2 size, glm::vec4 color);


    /// @brief Load a sub-rect of the texture.
    /// @param pixels The pixels are uploaded and should be row-major, origin in bottom-left.
    void load(Recti rect, const void *pixels, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

    static void bind(const Texture* texture);
    void bind() const;

    /// @brief Get the size of the texture in pixel.
    glm::vec2 getSize() const;

    unsigned int getID() const;

    enum Filter {
        Nearest, ///< GL_NEAREST
        Linear ///< GL_LINEAR
    };

    /// @brief Set the filtering method. Default's to nearest.
    void setFilter(Filter filter);

private:
    static std::weak_ptr<const Texture> m_defaultTexture;

    GL::Texture m_texture;
};

