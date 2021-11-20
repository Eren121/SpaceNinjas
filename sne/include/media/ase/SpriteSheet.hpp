#pragma once

#include "Frame.hpp"
#include <logging.hpp>
#include <map>

namespace Snow::media::ase
{
    /// @brief Represent the parsing output of a aseprite .json sprite sheet.
    /// @details
    ///     Format:
    ///       - "frames": object...
    ///         - "<img_path>"
    ///           - frame: { x, y, w, h } (rect)
    ///           - duration: count of milliseconds
    /// @remarks The spritesheet should be exported as "Array" in Aseprite
    class SpriteSheet
    {
        LOGGABLE("ase::SpriteSheet")

    public:
        /// @brief Load the sprite sheet from a json file path
        SpriteSheet() = default;
        explicit SpriteSheet(const fs::path& path);

        const std::vector<Frame>& getFrames() const { return m_frames; }

        Time getTotalDuration() const { return m_totalDuration; }

        const Frame& getFrameFromTime(Time time) const;

    private:
        std::vector<Frame> m_frames;

        /// @brief Indexing for fast-access from time values
        /// @details
        ///     key = Time of the end of the frame
        ///     value = index of the frame
        std::map<Time, int> m_byTime;

        Time m_totalDuration;
    };
}

namespace Snow
{
    using SpriteSheet = media::ase::SpriteSheet;
}