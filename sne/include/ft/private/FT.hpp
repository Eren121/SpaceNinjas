#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace FT
{
    /// @brief Reverse a bitmap vertically
    void reverseBitmapVertically(FT_Bitmap& bitmap);
}