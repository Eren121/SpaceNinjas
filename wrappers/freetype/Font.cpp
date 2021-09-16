#include "Font.hpp"
#include "private/Face.hpp"
#include "private/raii/Library.hpp"
#include <utility>

Font::Font(std::filesystem::path fontPath)
    : m_library(std::make_unique<FT::RAII::Library>()),
      m_fontPath(std::move(fontPath))
{
}

// We still need to implement the destructor in .cpp because the destruction can't happen in header,
// Some types are incomplete
Font::Font() = default;
Font::~Font() = default;
Font::Font(Font&&) noexcept = default;
Font& Font::operator=(Font&&) noexcept = default;

SDL_version Font::getFreetypeCompiledVersion()
{
    return FT::RAII::Library::getCompiledVersion();
}

SDL_version Font::getFreetypeLinkedVersion() const
{
    return m_library->getLinkedVersion();
}

const FT::Face& Font::getFace(FontType type) const
{
    if(!m_faces.contains(type))
    {
        m_faces[type] = type.makeFace(*m_library, m_fontPath);
    }

    return *m_faces.at(type);
}

