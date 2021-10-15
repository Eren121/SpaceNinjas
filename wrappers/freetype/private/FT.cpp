#include "FT.hpp"

#include <vector>
#include <algorithm>

namespace FT
{
    void reverseBitmapVertically(FT_Bitmap& bitmap)
    {
        std::vector<unsigned char> rowBuffer(bitmap.pitch); // Temporary buffer to store the row

        for(int row = 0; row < static_cast<int>(bitmap.rows / 2); ++row)
        {
            // Flip each row
            // rows / 2 round to lower, so if there is a odd number of row the center row will not be swapped
            // which is ok because it do not need to

            unsigned char *row1 = bitmap.buffer + bitmap.pitch * row;
            unsigned char *row2 = bitmap.buffer + bitmap.pitch * (bitmap.rows - 1 - row);

            std::swap_ranges(row1, row1 + bitmap.pitch, row2);
        }
    }
}
