#include "Game.hpp"
#include "PlayerControl.hpp"
#include <utility/math.hpp>

void PlayerControl::update(b2Body& player)
{
    tryMove(player);
    tryRotateWithKeys(player);

    // Restrict the player to the visible area
    clampPlayer(player);
}

void PlayerControl::tryMove(b2Body& player)
{
    glm::vec2 move = m_stage.getGame().getControls().move.getValue();

    if(move.x != 0.0f || move.y != 0.0f) // Use different because we know the default is really exact 0.0f
    {
        // Quantity of force applied, see https://en.wikipedia.org/wiki/Momentum
        float momentum = 10.0f;

        player.ApplyLinearImpulseToCenter(b2::fromGLM(momentum * move), true);
    }
}

void PlayerControl::clampPlayer(b2Body& player)
{
    glm::vec2 pos = b2::getPosition(player);

    glm::vec2 posMin = convertClipSpaceToSimulationSpace({-1.0f, -1.0f});
    glm::vec2 posMax = convertClipSpaceToSimulationSpace({1.0f, 1.0f});

    glm::vec2 clampedPos = glm::clamp(pos, posMin, posMax);

    if(pos != clampedPos)
    {
        b2::setPosition(player, clampedPos);
    }
}

void PlayerControl::tryRotateWithKeys(b2Body& player)
{
    glm::vec2 dir = m_stage.getGame().getControls().turn.getValue();
    
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
    glm::vec2 mouse = m_stage.getGame().getWindow().getInput().getMouseInClipSpace();

    // Transform from clip space to simulation space
    // P*V*M*X = X' (M=id)
    // X' = V^-1 * P^-1 * X
    mouse = convertClipSpaceToSimulationSpace(mouse);

    float angle = math::vec2angle(mouse - b2::toGLM(player.GetPosition()));

    b2::setAngle(player, angle);
}

glm::vec2 PlayerControl::convertClipSpaceToSimulationSpace(const glm::vec2& clipCoords) const
{
    auto& game = m_stage.getGame();
    return glm::inverse(game.getViewMatrix())
         * glm::inverse(game.getProjectionMatrix())
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
