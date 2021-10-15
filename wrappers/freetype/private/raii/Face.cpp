#include "Face.hpp"
#include "wrappers/freetype/private/Check.hpp"
#include <iostream>

namespace FT::RAII
{
    Face::Face(FT_Library library, const std::filesystem::path &path, int characterSize)
    {
        std::cout << "Loading font '" << path << "' with character size " << characterSize << "px" << std::endl;

        FT_Check(FT_New_Face(library, path.string().c_str(), 0, &m_wrapped));
        FT_Check(FT_Set_Pixel_Sizes(m_wrapped, 0, characterSize));
    }

    Face::~Face() noexcept(false)
    {
        if(m_wrapped)
        {
            FT_Check(FT_Done_Face(m_wrapped));
            m_wrapped = nullptr;
        }
    }

    Face::operator FT_Face() const
    {
        return m_wrapped;
    }

    FT_Face Face::operator->() const
    {
        return m_wrapped;
    }
}