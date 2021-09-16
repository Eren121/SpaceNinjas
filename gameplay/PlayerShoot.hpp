#pragma once

#include "PlayerProcess.hpp"
#include <utility/time/Clock.hpp>

class Game;

class PlayerShoot : public PlayerProcess
{
public:
    explicit PlayerShoot(Stage& stage);

    /// @brief Minimal delay between shots
    Time delay;

protected:
    void update(b2Body& player) override;
    bool tryShoot(b2Body& player);

private:
    Clock m_delayClock;
};

