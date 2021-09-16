#include "UIStage.hpp"
#include "Game.hpp"
#include <utility/Str.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <imgui.h>

UIStage::UIStage(Game& game, Stage& stage)
    : m_game(game),
      m_stage(stage)
{
}

bool UIStage::updateNode()
{
    if(m_frame == 0)
    {
        // There may be loading time (textures, etc...) between the constructor and the first frame
        // So we measure only since here
        m_sinceFirstFrame.restart();
    }

    m_frame++;
    m_fpsCounter.onFrame();
    
    return true;
}

void UIStage::drawNode(RenderStates states) const
{
}

void UIStage::debugNode()
{
    if(ImGui::CollapsingHeader("Simulation metadata"))
    {
        // ImGUI needs static variable to work
        int iteration = m_stage.getWorld().getIteration();
        float time = m_stage.getWorld().getTime().asSeconds();
        float fps = m_fpsCounter.getFPS();
        
        ImGui::InputInt("Iteration", &iteration, 0, 0, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat("Time", &time, 0.0f, 0.0f, "%.3fs", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat("FPS (iter/s)", &fps, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
    }
    
    if(ImGui::CollapsingHeader("Simulation game"))
    {
        int stage = m_stage.getID();
        
        // Change the stage with this input
        if(ImGui::InputInt("Stage", &stage, 1, 0))
        {
            // We know the top is the Stage because this UI lives in the stage
            m_game.scene.pop();
            
            m_game.pushSceneStage(stage);
        }
        
        if(m_stage.hasPlayer())
        {
            static glm::vec2 pos; pos = b2::toGLM(m_stage.getPlayer().GetPosition());
            
            if(ImGui::InputFloat2("Player position", &pos.x))
            {
                b2::setPosition(m_stage.getPlayer(), pos);
            }
        }
    }
}

