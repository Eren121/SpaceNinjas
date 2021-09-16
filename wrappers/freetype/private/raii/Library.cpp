#include "Library.hpp"
#include "wrappers/freetype/private/Check.hpp"

namespace FT::RAII
{
    Library::Library()
    {
        FT_Check(FT_Init_FreeType(&m_wrapped));
    }

    Library::~Library() noexcept(false)
    {
        if(m_wrapped)
        {
            FT_Check(FT_Done_FreeType(m_wrapped));
            m_wrapped = nullptr;
        }
    }

    Library::operator FT_Library() const
    {
        return m_wrapped;
    }

    SDL_version Library::getCompiledVersion()
    {
        SDL_version ret;
        ret.major = FREETYPE_MAJOR;
        ret.minor = FREETYPE_MINOR;
        ret.patch = FREETYPE_PATCH;

        return ret;
    }

    SDL_version Library::getLinkedVersion() const
    {
        FT_Int major, minor, patch;
        FT_Library_Version(m_wrapped, &major, &minor, &patch);

        SDL_version ret;
        ret.major = major;
        ret.minor = minor;
        ret.patch = patch;

        return ret;
    }
}
