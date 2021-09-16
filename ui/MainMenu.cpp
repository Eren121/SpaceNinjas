#include "MainMenu.hpp"
#include "Game.hpp"
#include "media/input/Axis.hpp"
#include "process/Wait.hpp"
#include "utility/math.hpp"
#include "utility/time/Timer.hpp"
#include "wrappers/freetype/Text.hpp"
#include <imgui.h>

namespace ui
{
    MainMenu::MainMenu(Game& game)
        : m_game(game)
    {
    }
    
    void MainMenu::drawNode(RenderStates states) const
    {
        const Window& window = m_game.getWindow();
        const glm::vec2 winSize = window.getSize();
        
        states.model = glm::mat4(1.0f);
        states.view = glm::ortho(0.0f, winSize.x, 0.0f, winSize.y);
    
        int charSize = 64;
        
        for(int i = 0; i < static_cast<int>(m_inputs.size()); ++i)
        {
            const Input& input = m_inputs.at(i);
            
            glm::vec2 pos;
            pos.x = winSize.x / 2.0f;
            pos.y = winSize.y / (m_inputs.size() + 1.0f) * static_cast<float>(i + 1);
            pos.y = winSize.y - pos.y;
            
            Text text;
            text.setFont(&m_game.getFont());
            text.setCharacterSize(charSize);
            text.setString(input.label);
            text.setPosition(pos);
            text.setOrigin(Text::Centered); // Centerize the text vertically and horizontally
            text.setOutline(true);
            text.setOutlineColor({1.0f, 0.0f, 1.0f, 0.5f});
            
            if(i == m_focus)
            {
                text.setColor({1.0f, 0.0f, 1.0f, 0.5f});
            }
            
            text.draw(states);
        }
    }
    
    void MainMenu::addOption(const std::string& label, std::function<void()> onClick)
    {
        m_inputs.push_back(Input{
            .label = label,
            .onClick = std::move(onClick)
        });
    }
    
    bool MainMenu::updateNode()
    {
        return (*this)();
    }
    
    task<> MainMenu::coroutine()
    {
        const GameControls& controls = m_game.getControls();
    
        // Wait before allowing to move again
        Timer beforeMove;
        
        while(true)
        {
            if(beforeMove.elapsed())
            {
                const glm::vec2 move = controls.menuMove.getValue();
    
                if(move.y != 0)
                {
                    if(m_focus == Unfocused)
                    {
                        // Treat specifically the case it's the first user input
                        // If he goes down, select first item (on top), like if he scroll down from outside the screen
                        // If he goes up, select last item (on bottom)
                        
                        if(move.y > 0)
                        {
                            // He goes up
                            m_focus = static_cast<int>(m_inputs.size()) - 1;
                        }
                        else
                        {
                            // He goes down
                            m_focus = 0;
                        }
                    }
                    else
                    {
                        // Move the cursor depending of the player movement
                        // As the menu origin is top, -y add one element, so we have to negate the sign
                        m_focus += -math::sgn(move.y);
    
                        // Stay in the range [0;nbItem-1]
                        m_focus = math::positive_mod<int>(m_focus, m_inputs.size());
                    }
                    
                    beforeMove = Time::milliseconds(400);
                }
            }
            
            if(controls.menuSelect.isJustPressed())
            {
                onClick();
            }
            
            co_await std::suspend_always{};
        }
    }
    
    void MainMenu::debugNode()
    {
        if(ImGui::CollapsingHeader("Main menu"))
        {
            ImGui::InputInt("Focus", &m_focus, 1, 100);
        }
    }
    
    void MainMenu::onClick()
    {
        if(m_focus >= 0 && m_focus < static_cast<int>(m_inputs.size()))
        {
            const auto& callback = m_inputs.at(m_focus).onClick;
            
            if(callback)
            {
                callback();
            }
        }
    }
}