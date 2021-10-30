#include "gameplay/player/PlayerShoot.hpp"
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
        Game &game = getStage().getGame();
        b2::World &world = getStage().getWorld();

        bool hasShot = false;

        if (game.getControls().shoot.isPressed())
        {
            // Speed of the missile, in m/s
            const float speed = 100.0f;

            // Use the same direction as the player is facing
            const glm::vec2 direction = math::angle2vec(player.GetAngle());

            const Texture *playerTexture = &game.textures("laser.png");
            const glm::vec2 playerTextureSize = playerTexture->getSize();
            const float ratio = playerTextureSize.y / playerTextureSize.x;
            const float fixedSizeX = 2.0f;

            Rect box;
            box.size = {fixedSizeX, fixedSizeX * ratio};
            box.setCenter(b2::toGLM(player.GetPosition()) + direction);

            auto &missile = world.createBoxBody(box, b2_dynamicBody, 1.0f, true);

            missile.SetLinearVelocity(b2::fromGLM(direction * speed));
            b2::setAngle(missile, player.GetAngle());

            Body *user = new Body(Body::PlayerMissile, missile, box.size / 2.0f);
            user->setTexture(playerTexture);
            missile.GetUserData() = user;

            hasShot = true;
        }

        return hasShot;
    }
}