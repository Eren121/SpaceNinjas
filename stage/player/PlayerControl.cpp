#include "stage/player/PlayerControl.hpp"
#include "Game.hpp"
#include "utility/math.hpp"

namespace SpaceNinja
{
    void PlayerControl::onStep(b2Body& player)
    {
        tryMove(player);

        //tryRotateWithKeys(player);
        tryRotateFollowMouse(player);
    }

    void PlayerControl::tryMove(b2Body& player)
    {
        glm::vec2 moveDirection = getStage().getGame().getControls().move.getValue();

        if(moveDirection.x != 0.0f || moveDirection.y != 0.0f) // Use different because we know the default is really exact 0.0f
        {
            // Quantity of force applied
            float force = 100.0f;

            // NOT use impulse
            // Because impulse is dependant of how much time I call it.
            // It is better for one-time impulsion.
            // For being independant to timestep (delta), force is better
            // The unit is different but finally they are more or less the same thing.

            //player.ApplyLinearImpulseToCenter(b2::fromGLM(momentum * move), true);
            player.ApplyForceToCenter(b2::fromGLM(force * moveDirection), true);

            const float maxSpeed = 20.0f;

            if(player.GetLinearVelocity().LengthSquared() > maxSpeed * maxSpeed)
            {
                auto speed = player.GetLinearVelocity();
                speed.Normalize();
                speed.x *= maxSpeed;
                speed.y *= maxSpeed;

                player.SetLinearVelocity(speed);
            }
        }
    }

    void PlayerControl::tryRotateWithKeys(b2Body& player)
    {
        glm::vec2 dir = getStage().getGame().getControls().turn.getValue();

        if(dir.x != 0 || dir.y != 0)
        {
            float currentAngle = player.GetAngle();
            float targetAngle = math::vec2angle(dir);

            float nextAngle = math::angularMix(currentAngle, targetAngle, 0.1f);

            b2::setAngle(player, nextAngle);
        }
    }

    void PlayerControl::tryRotateFollowMouse(b2Body& player)
    {
        // The player's ship always face the mouse position

        // Get the mouse in simulation space
        glm::vec2 mouse = getStage().getGame().getWindow().getInput().getMouseInClipSpace();

        // Transform from clip space to simulation space
        // P*V*M*X = X' (M=id)
        // X' = V^-1 * P^-1 * X
        mouse = convertClipSpaceToSimulationSpace(mouse);

        float angle = math::vec2angle(mouse - b2::toGLM(player.GetPosition()));

        b2::setAngle(player, angle);
    }

    glm::vec2 PlayerControl::convertClipSpaceToSimulationSpace(const glm::vec2& clipCoords) const
    {
        return glm::inverse(getStage().getViewMatrix())
             * glm::vec4(clipCoords, 0.0f, 1.0f);
    }

    glm::vec2 PlayerControl::getDirection(SDL_Scancode left, SDL_Scancode right, SDL_Scancode up, SDL_Scancode down) const
    {
        const Uint8 *state = SDL_GetKeyboardState(nullptr);
        glm::vec2 move(0.0f, 0.0f);

        if(state[up])
        {
            move.y++;
        }
        if(state[down])
        {
            move.y--;
        }
        if(state[left])
        {
            move.x--;
        }
        if(state[right])
        {
            move.x++;
        }

        return move;
    }
}