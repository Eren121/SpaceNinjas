#include "gl/Texture.hpp"
#include "media/SDL.hpp"
#include <io.hpp>
#include <glm/vec4.hpp>

Texture::Texture()
{
}

void Texture::load1x1White()
{
    static const GLubyte pixels[] = {
        0xff, 0xff, 0xff, 0xff // RGBA Opaque white 1x1
    };

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::loadMissingTexture()
{
    const glm::ivec2 size {2, 2};
    const glm::vec4 pink {0.5f, 0, 0.5f, 1};
    const glm::vec4 black{0, 0, 0, 1};
    const Recti rect{size};

    std::vector<glm::vec4> pixels(size.x * size.y, black);
    pixels[0] = pink;
    pixels[3] = pink;

    load(size);
    load(rect, pixels.data(), GL_RGBA, GL_FLOAT);
    setFilter(Nearest);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::load(const std::filesystem::path& path)
{
    if(!fs::exists(path))
    {
        throw snk::file_not_found_exception(path);
    }

    SDL_Surface *image = IMG_Load(path.string().c_str());
    if(!image)
    {
        throw snk::io_exception{snk::str{} << path << ": " << SDL_GetError()};
    }

    load(image);
    SDL_FreeSurface(image);
}

void Texture::load(SDL_Surface *surface)
{
    // Note that the format could be any format, we need to convert to a format we know to tell the format to OpenGL
    // We convert to RGBA unsigned char
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    if(!format)
    {
        throw SDL::Exception("Failed to allocate pixel format");
    }

    SDL_Surface *converted = SDL_ConvertSurface(surface, format, 0);
    if(!converted)
    {
        throw SDL::Exception("Failed to convert surface");
    }

    SDL_FreeFormat(format);

    SDL::flip_vertically(converted);

    // Neeeded if we want to directly access the pixels
    if(SDL_LockSurface(converted) != 0)
    {
        throw SDL::Exception("Failed to lock texture");
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted->w, converted->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_UnlockSurface(converted);

    SDL_FreeSurface(converted);
}

void Texture::load(glm::ivec2 size, glm::vec4 color)
{
    std::vector<glm::vec4> pixels(size.x * size.y, color);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_FLOAT, pixels.data());
}

void Texture::load(glm::ivec2 size)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
}

void Texture::load(Recti rect, const void *pixels, GLenum format, GLenum type)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, rect.origin.x, rect.origin.y, rect.size.x, rect.size.y, format, type, pixels);
}

void Texture::bind(const Texture *texture)
{
    if(texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture->m_texture);
    }
    else
    {
        static Texture defaultTex;
        static bool initialized = false;

        if(!initialized)
        {
            initialized = true;
            defaultTex.load1x1White();
        }

        glBindTexture(GL_TEXTURE_2D, defaultTex.m_texture);
    }
}

void Texture::bind() const
{
    bind(this);
}

unsigned int Texture::getID() const
{
    return m_texture.id;
}

void Texture::setFilter(Texture::Filter filter)
{
    int gl_filter;

    switch(filter)
    {
        case Linear:
            gl_filter = GL_LINEAR;
            break;

        case Nearest:
            gl_filter = GL_NEAREST;
            break;
    }

    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter);
}

glm::vec2 Texture::getSize() const
{
    int miplevel = 0;
    int w, h;

    bind();
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

    return {w, h};
}