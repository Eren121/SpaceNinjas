#include "media/ase/SpriteSheet.hpp"
#include <json.hpp>
#include <io.hpp>
#include <fstream>
#include <cassert>

namespace Snow::media::ase
{
    SpriteSheet::SpriteSheet(const fs::path& path)
    {
        if(!fs::exists(path) && false)
        {
            getLogger().error("File not found: {}", path);
            throw snk::file_not_found_exception{path};
        }

        const fs::path workingDirectory{path.parent_path()};

        std::ifstream ifs{path};
        json j;
        ifs >> j;

        // Json object maps to std::map
        // More on https://json.nlohmann.me/features/types/

        const json& j_frames{j.at("frames")};
        int index = 0;

        for(const json& j_frame : j_frames)
        {
            const std::string& filename = j_frame.at("filename");
            Frame& frame{m_frames.emplace_back()};

            // Store index of the frame
            frame.index = index;

            // Store the path of the image associated to the frame
            // Most probably the same for all the frames
            frame.imagePath = workingDirectory / filename;
            frame.filename = filename;

            // Parse and store the texture rect as {x, y, w, h}
            json j_rect{j_frame.at("frame")};
            Rect rect;
            rect.origin.x = j_rect["x"];
            rect.origin.y = j_rect["y"];
            rect.size.x = j_rect["w"];
            rect.size.y = j_rect["h"];

            frame.rect = rect;

            // Store the duration
            // Stored in the json as a float in milliseconds unit
            Time duration{Time::milliseconds(j_frame["duration"])};
            frame.duration = duration;

            m_totalDuration += duration;
            m_byTime[m_totalDuration] = index;

            index++;
        }

        const char* f = "Loading sheet {} with {} frames and total duration of {}";
        getLogger().info(f, path, m_frames.size(), m_totalDuration);
    }

    const Frame &SpriteSheet::getFrameFromTime(Time time) const
    {
        // Keep in the duration,
        // So the map should always have a key
        // If it has, it is empty

        if(time >= m_totalDuration)
        {
            time %= m_totalDuration;
        }

        // Get the first frame which time is >= requested time
        auto iterator{m_byTime.upper_bound(time)};

        if(iterator == m_byTime.end())
        {
            // Return static null frame
            // If the sprite sheet is empty, this is normal
            // If the sprite sheet is not empty, this should not happen:
            // The time is always lower than the max key with the modulo

            static Frame nullFrame{};

            if(!m_frames.empty())
            {
                const char *f = "Can't find the Frame in the non-empty sprite sheet, "
                                "it shouldn't have happened";
                getLogger().warn(f);
            }

            return nullFrame;
        }
        else
        {
            return m_frames[iterator->second];
        }
    }
}