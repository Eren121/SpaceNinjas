#pragma once

#include "stage/player/PlayerSceneNode.hpp"
#include "utility/time/Timer.hpp"

namespace SpaceNinja
{
    class PlayerShoot : public PlayerSceneNode
    {
    public:
        explicit PlayerShoot(Stage &stage, Time delay = Time::milliseconds(50));

        void setDelay(Time delay);
        Time getDelay() const;

    protected:
        void onStep(b2Body &player) override;

        bool tryShoot(b2Body &player);

    private:

        /// @brief Used to check if the time has elapsed.
        /// @remarks Not a Clock/Timer because simulation-time is not always IRL time.
        Time m_lastUpdate;

        /// @brief Minimal delay between shots
        Time m_delay;
    };
}
