#pragma once

#include <cereal/cereal.hpp>
#include <filesystem>

/// @brief Represents all the player data stored in a save file.
struct Save
{
    /// @brief Contains the last level the player has unlocked and can play.
    int lastLevelUnlocked{1};
    
    template<typename Archive>
    void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(lastLevelUnlocked));
    }
};

