#pragma once

#include "media/scene/SceneNode.hpp"
#include <snk/logging.hpp>
#include <cereal/cereal.hpp>
#include <filesystem>

/// @brief Remove the "m_" prefix for the name
#define CEREAL_NVP_M(field) ::cereal::make_nvp(&#field[2], field)

/// @brief Represents all the player data stored in a save file.
struct Save : public SceneNode
{
    LOGGABLE("Save")

private:
    /// @brief Contains the last level the player has unlocked and can play.
    /// @remarks Starts at 1.
    int m_lastLevelUnlocked{1};

protected:
    void debugNode() override;

public:
    template<typename Archive>
    void save(Archive& ar) const
    {
        ar(CEREAL_NVP_M(m_lastLevelUnlocked));
    }
    
    template<typename Archive>
    void load(Archive& ar)
    {
        ar(CEREAL_NVP_M(m_lastLevelUnlocked));
    }

    int getLastLevelUnlocked() const;
    void setLastLevelUnlocked(int i);
};