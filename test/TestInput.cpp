#include "TestInput.hpp"
#include "media/input/BoolAction.hpp"
#include "media/input/AxisAction.hpp"
#include "media/input/VectorAction.hpp"

#include <imgui.h>

namespace SpaceNinja::test
{

TestInput::TestInput()
{
    m_window.onEvent.connect([this](const SDL_Event &e, long) {
        switch (e.type) {
            case SDL_KEYDOWN:
                m_lastEvent = snk::str{} << "Key down: " << SDL_GetKeyName(e.key.keysym.sym);
                break;

            case SDL_KEYUP:
                m_lastEvent = cat("Key up: ", SDL_GetKeyName(e.key.keysym.sym));
                break;

            case SDL_CONTROLLERBUTTONDOWN:
                m_lastEvent = cat("Game Controller button down: ",
                                  SDL_GameControllerGetStringForButton(
                                          static_cast<SDL_GameControllerButton>(e.cbutton.button)));
                break;

            case SDL_CONTROLLERBUTTONUP:
                m_lastEvent = cat("Game Controller button up: ",
                                  SDL_GameControllerGetStringForButton(
                                          static_cast<SDL_GameControllerButton>(e.cbutton.button)));
                break;

            case SDL_CONTROLLERDEVICEADDED:
                m_lastEvent = "Controller added";
                if (!m_gameController) {
                    m_gameController = SDL::GameController(e.cdevice.which);
                }
                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                m_lastEvent = "Controller removed";
                if (e.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                    // If the removed controller is m_gameController, reset m_gameController
                    m_gameController = {};
                }
                break;

            case SDL_CONTROLLERAXISMOTION:
                auto axis = static_cast<SDL_GameControllerAxis>(e.caxis.axis);

                Sint16 intensity = SDL_GameControllerGetAxis(m_gameController, axis);
                m_lastEvent = cat("Axis ", SDL_GameControllerGetStringForAxis(axis), " motion is ", intensity);

                break;
        }
    });

    shoot.setInput(&m_window.getInput());
    jump.setInput(&m_window.getInput());
    move.setInput(&m_window.getInput());

    shoot.bindKey(SDLK_a);
    shoot.bindKey(SDLK_z);
    shoot.bindKey(SDL_CONTROLLER_BUTTON_A);
    shoot.bindKey(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    shoot.bindAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT);

    jump.bindAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    jump.bindKey(SDLK_e);

    move.bindAxes(SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY);
    move.bindKeys(SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN, SDLK_UP);
}

void TestInput::draw()
{
    ImGui::Text("%s", m_lastEvent.c_str());

    if (m_gameController) {
        ImGui::Text("Joystick %s, instance_id=%d",
                    SDL_GameControllerName(m_gameController),
                    SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController)));
    }

    auto header = [](const char *label, Snow::in::Action &action) {
        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("isPressed(): %d", action.isPressed());
            ImGui::Text("isJustPressed(): %d", action.isJustPressed());
            ImGui::Text("isJustReleased(): %d", action.isJustReleased());
            ImGui::Text("asFloat(): %f", action.asFloat());
        }
    };

    header("Shoot", shoot);
    header("Jump", jump);

    if (ImGui::CollapsingHeader("Move", ImGuiTreeNodeFlags_DefaultOpen)) {
        static glm::vec2 value;
        value = move.getValue();
        ImGui::InputFloat2("Value", &value.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
    }
}

}