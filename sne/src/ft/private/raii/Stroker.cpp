#include "ft/private/raii/Stroker.hpp"
#include "ft/private/Check.hpp"

namespace FT::RAII
{
    Stroker::Stroker(FT_Library library)
    {
        FT_Check(FT_Stroker_New(library, &m_wrapped));
    }

    Stroker::~Stroker()
    {
        if(m_wrapped)
        {
            FT_Stroker_Done(m_wrapped);
            m_wrapped = nullptr;
        }
    }

    Stroker::operator FT_Stroker() const
    {
        return m_wrapped;
    }
}
