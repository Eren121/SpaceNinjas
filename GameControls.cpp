#include "GameControls.hpp"
#include <imgui.h>

GameControls::GameControls(UnifiedInput& input)
{
    listenGameControllerChange(input);
    
    for(auto *action : {&shoot, &menuSelect, &menuBack, &showDebugWindow})
    {
        action->setInput(&input);
    }
    
    for(auto *action : {&move, &turn, &menuMove})
    {
        action->setInput(&input);
    }
    
    showDebugWindow.bindKey(SDLK_F12);
    
    shoot.bindAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    shoot.bindKey(SDLK_SPACE);
    
    move.bindAxes(SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY);
    turn.bindAxes(SDL_CONTROLLER_AXIS_RIGHTX, SDL_CONTROLLER_AXIS_RIGHTY);
    
    menuMove.bindKeys(SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN, SDLK_UP);
    menuSelect.bindKey(SDLK_SPACE);
    menuBack.bindKey(SDLK_ESCAPE);
    
    for(auto& x : {&move, &turn})
    {
        x->bindKeys(SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN, SDLK_UP);
    }
}

void GameControls::debugNode()
{
    if(ImGui::CollapsingHeader("Controls"))
    {
        std::string controller;
        if(m_gameController)
        {
            controller = SDL_GameControllerName(m_gameController);
        }
        
        ImGui::InputText("Controller", controller.data(), controller.size(), ImGuiInputTextFlags_ReadOnly);
        
        bool shooting = shoot.isPressed();
        ImGui::Checkbox("shoot", &shooting);
        
        glm::vec2 moving = move.getValue();
        ImGui::InputFloat2("move", &moving.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
        
        glm::vec2 turning = turn.getValue();
        ImGui::InputFloat2("turn", &turning.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
    }
}

void GameControls::listenGameControllerChange(UnifiedInput& input)
{
    sigslot::scoped_connection conn;
    
    conn = input.onEventType[SDL_CONTROLLERDEVICEADDED].connect([this](InputListener::Source source) {
        if(!m_gameController)
        {
            m_gameController = SDL::GameController(source.event.cdevice.which);
            SDL_JoystickSetPlayerIndex(SDL_GameControllerGetJoystick(m_gameController), 0);
        }
    });
    m_connections.emplace_back(std::move(conn));
    
    conn = input.onEventType[SDL_CONTROLLERDEVICEREMOVED].connect([this](InputListener::Source source) {
        const SDL_Event& e = source.event;
        SDL_Joystick *const joystick = SDL_GameControllerGetJoystick(m_gameController);
        const int currentInstanceID = SDL_JoystickInstanceID(joystick);
        
        if(e.cdevice.which == currentInstanceID)
        {
            // If the removed controller is m_gameController, reset m_gameController
            m_gameController = {};
            SDL_JoystickSetPlayerIndex(nullptr, 0);
        }
    });
    m_connections.emplace_back(std::move(conn));
}
