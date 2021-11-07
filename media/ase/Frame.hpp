#pragma once

#include <snk/filesystem.hpp>
#include <snk/Rect.hpp>
#include <snk/time/Time.hpp>

namespace Snow::media::ase
{
    struct Frame
    {
        /// @brief Index of the frame
        int index;

        /// @brief Path of the image for this frame
        fs::path imagePath;

        /// @brief 'filename' argument of the frame
        std::string filename;

        /// @brief UV Texture rect of the frame in pixels, NOT in normalized units.
        /// @remarks Care because the shader needs the rect to be in normalized units, so you will have
        /// to divide by the size of the texture.
        Rect rect;

        Time duration;
    };
}