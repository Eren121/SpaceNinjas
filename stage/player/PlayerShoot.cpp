#include "stage/player/PlayerShoot.hpp"
#include "Game.hpp"
#include "utility/math.hpp"

namespace SpaceNinja
{
    PlayerShoot::PlayerShoot(Stage &stage, Time delay)
            : PlayerSceneNode{stage},
              m_delay{delay}
    {
    }

    void PlayerShoot::setDelay(Time delay)
    {
        m_delay = delay;
    }

    Time PlayerShoot::getDelay() const
    {
        return m_delay;
    }

    void PlayerShoot::onStep(b2Body &player)
    {
        const Time worldTime = getWorld().getTime();

        if (m_lastUpdate + m_delay <= worldTime && tryShoot(player))
        {
            m_lastUpdate = worldTime;
        }
    }

    bool PlayerShoot::tryShoot(b2Body &player)
    {
        Game& game{getStage().getGame()};
        StageWorld& world{getStage().getWorld()};

        bool hasShot{false};

        if (game.getControls().shoot.isPressed())
        {
            const glm::vec2 pos{b2::getPosition(player)};

            // Speed of the missile, in m/s
            const float speed{100.0f};

            // Use the same direction as the player is facing
            const glm::vec2 direction{math::angle2vec(player.GetAngle())};

            b2Body &missile{world.createMissileBody(pos + direction)};
            b2::setVelocityWithAngle(missile, speed * direction);

            hasShot = true;
        }

        return hasShot;
    }
}