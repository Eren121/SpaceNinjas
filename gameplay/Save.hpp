#pragma once

#include "media/scene/SceneNode.hpp"
#include <cereal/cereal.hpp>
#include <filesystem>

/// @brief Represents all the player data stored in a save file.
struct Save : public SceneNode
{
    /// @brief Contains the last level the player has unlocked and can play.
    int lastLevelUnlocked{1};
    
    template<typename Archive>
    void save(Archive& ar) const
    {
        ar(CEREAL_NVP(lastLevelUnlocked));
    }
    
    template<typename Archive>
    void load(Archive& ar)
    {
        ar(CEREAL_NVP(lastLevelUnlocked));
    }

protected:
    void debugNode() override;
};