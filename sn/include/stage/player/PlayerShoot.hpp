#pragma once

#include "PlayerSceneNode.hpp"
#include <time/Timer.hpp>

namespace sn
{
    class PlayerShoot : public PlayerSceneNode
    {
    public:
        explicit PlayerShoot(Stage &stage, Time delay = Time::milliseconds(400));

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
