#pragma once

#include "utility/swapmove.hpp"
#include <ft2build.h>
#include FT_STROKER_H

namespace FT::RAII
{
    /// @brief RAII FT_Stroker.
    class Stroker
    {
    public:
        /// @brief Creates an invalid stroker
        Stroker() = default;

        /// @brief Creates the stroker.
        explicit Stroker(FT_Library library);

        /// @brief Destroys the stroker.
        ~Stroker();

        /// @brief Make moveable, but not copyable.
        SWAP_MOVABLE(Stroker, &Stroker::m_wrapped)

        operator FT_Stroker() const;

    private:
        FT_Stroker m_wrapped{nullptr};
    };
}