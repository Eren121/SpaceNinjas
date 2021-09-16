#include "PlayerShoot.hpp"
#include "Game.hpp"
#include <utility/math.hpp>

PlayerShoot::PlayerShoot(Stage& stage)
    : PlayerProcess(stage),
      delay(Time::milliseconds(400))
{

}

void PlayerShoot::update(b2Body& player)
{
    if(m_delayClock.getElapsedTime() > delay && tryShoot(player))
    {
        m_delayClock.restart();
    }
}

bool PlayerShoot::tryShoot(b2Body& player)
{
    auto& game = m_stage.getGame();
    auto& world = m_stage.getWorld();
    
    bool shot = false;
    
    if(game.getControls().shoot.isPressed())
    {
        float speed = 40.0f; // Speed of the missile, in m/s
        glm::vec2 direction = math::angle2vec(player.GetAngle());// Use the same direction as the player is facing

        Rect box;
        box.size = {1, 1};
        box.setCenter(b2::toGLM(player.GetPosition()) + direction);

        auto& missile = world.createBoxBody(box, b2_dynamicBody, 1.0f, true);

        missile.SetLinearVelocity(b2::fromGLM(direction * speed));
        b2::setAngle(missile, player.GetAngle());

        Body *user = new Body(Body::PlayerMissile, missile, box.size / 2.0f);
        user->setTexture(&game.textures("laser.png"));
        b2::setUserData(missile, user);
    
        shot = true;
    }

    return shot;
}