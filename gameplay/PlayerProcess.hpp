#pragma once

#include "media/scene/SceneNode.hpp"
#include "process/Process.hpp"
#include <wrappers/box2d/World.hpp>

class Game;
class Stage;
class Window;

/// @brief Process for a player behavior.
/// @details The Process will not update if the world has not player
class PlayerProcess : public SceneNode
{
public:
    explicit PlayerProcess(Stage& stage);

protected:
    bool updateNode() final;
    virtual void update(b2Body& player) = 0;

protected:
    Stage& m_stage;
};

