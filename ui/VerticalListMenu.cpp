#include "ui/VerticalListMenu.hpp"
#include "Game.hpp"
#include "media/input/Axis.hpp"
#include "process/Wait.hpp"
#include <snk/math.hpp>
#include <snk/time/Timer.hpp>
#include "wrappers/freetype/Text.hpp"
#include "GameControls.hpp"
#include <imgui.h>

namespace SpaceNinja::ui
{
    VerticalListMenu::VerticalListMenu(SpaceNinja::Game& game)
        : m_game(game)
    {
    }
    
    void VerticalListMenu::drawNode(RenderStates states) const
    {
        const glm::vec2 winSize = m_game.getUISize();
        
        states.model = glm::mat4(1.0f);
        states.view = m_game.getUIViewMatrix();
    
        int charSize = 64;
        
        for(int i = 0; i < count(); ++i)
        {
            const Input& input = m_inputs.at(i);
            
            glm::vec2 pos;
            pos.x = winSize.x / 2.0f;
            pos.y = winSize.y / (static_cast<float>(count()) + 1.0f) * static_cast<float>(i + 1);
            pos.y = winSize.y - pos.y;
            
            Text text;
            text.setFont(&m_game.getFont());
            text.setCharacterSize(charSize);
            text.setString(input.label);
            text.setPosition(pos);
            text.setOrigin(Text::Centered);
            text.setOutline(true);
            text.setOutlineColor({1.0f, 0.0f, 1.0f, 0.5f});
            
            if(i == m_focus)
            {
                text.setColor({1.0f, 0.0f, 1.0f, 0.5f});
            }
            
            text.draw(states);
        }
    }
    
    void VerticalListMenu::addOption(const std::string& label, Callback onClick)
    {
        m_inputs.push_back(Input{label, std::move(onClick)});
    }
    
    bool VerticalListMenu::updateNode()
    {
        return run();
    }
    
    task<> VerticalListMenu::coroutine()
    {
        const SpaceNinja::GameControls& controls = m_game.getControls();
    
        // Wait before allowing to move again
        Timer beforeMove;
        
        while(true)
        {
            if(beforeMove.elapsed())
            {
                const glm::vec2 move = controls.menuMove.getValue();

                if(move.y != 0)
                {
                    const int prevFocus = m_focus;

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
                        m_focus += -math::sgn(static_cast<int>(move.y));
    
                        // Stay in the range [0;nbItem-1]
                        m_focus = math::positive_mod<int>(m_focus, count());
                    }

                    if(prevFocus != m_focus)
                    {
                        // Cursor has moved
                    }
                    
                    beforeMove = Time::milliseconds(180);
                }
            }
            
            if(controls.menuSelect.isJustPressed())
            {
                onClick();
            }
            
            co_await std::suspend_always{};
        }
    }
    
    void VerticalListMenu::debugNode()
    {
        if(ImGui::CollapsingHeader("VerticalListMenu"))
        {
            ImGui::InputInt("Focus", &m_focus, 1, 100);
        }
    }
    
    void VerticalListMenu::onClick()
    {
        if(m_focus >= 0 && m_focus < count())
        {
            const auto& callback = m_inputs.at(m_focus).onClick;
            
            if(callback)
            {
                callback(*this);
            }
        }
    }
}