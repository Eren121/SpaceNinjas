#include "MenuStage.hpp"
#include "Game.hpp"
#include "wrappers/freetype/Text.hpp"
#include "utility/time/Timer.hpp"
#include "gameplay/Stage.hpp"
#include <imgui.h>

namespace ui
{
    MenuStage::MenuStage(Game &game)
        : m_game{game},
          m_lastLevelUnlocked{0}
    {
    }
    
    bool MenuStage::updateNode()
    {
        return (*this)();
    }
    
    task<> MenuStage::coroutine()
    {
        const GameControls& controls = m_game.getControls();
    
        // Wait before allowing to move again
        Timer beforeMove;
        
        while(true)
        {
            const auto move = [&]() {
                glm::ivec2 move = controls.menuMove.getValue();
    
                // Negate Y since the elements in menu are to-down
                move.y = -move.y;
                
                // Get only the sign
                move = glm::sign(move);
                
                return move;
            }();
    
            if(controls.menuBack.isJustPressed())
            {
                m_game.scene.pop();
            }
            
            if(beforeMove.elapsed() && (move.x != 0 || move.y != 0))
            {
                glm::ivec2 pos = *m_focus + move;
                
                // Stay in bounds
                pos.y = glm::clamp(pos.y, 0, m_rows - 1);
                pos.x = glm::clamp(pos.x, 0, m_columns - 1);
    
                // The last row may be incomplete
                if(pos.y == m_rows - 1)
                {
                    pos.x = glm::clamp<int>(pos.x, 0, (m_lastLevelUnlocked % m_columns) - 1);
                }
                
                // Unfocus previous item and focus new item (if they are the same it's ok)
                m_items.at(m_focus.getIndex())->setFocused(false);
                *m_focus = pos;
                m_items.at(m_focus.getIndex())->setFocused(true);
                
                beforeMove = Time::milliseconds(400);
            }
            
            if(controls.menuSelect.isJustPressed())
            {
                m_game.scene.push(std::make_shared<Stage>(m_game, m_focus.getIndex() + 1));
            }
            
            co_await std::suspend_always{};
        }
    }
    
    void MenuStage::drawNode(RenderStates states) const
    {
        const glm::vec2 winSize = m_game.getWindow().getSize();
        
        states.model = glm::mat4(1.0f);
        states.view = glm::ortho(0.0f, winSize.x, 0.0f, winSize.y);
    
        for(const auto& item : m_items)
        {
            item->draw(states);
        }
    }
    
    void MenuStage::debugNode()
    {
        if(ImGui::CollapsingHeader("Stage menu"))
        {
            int id = m_focus.getIndex();
            
            ImGui::InputInt2("Focus position" , &m_focus->x, ImGuiInputTextFlags_ReadOnly);
            ImGui::InputInt("Focus ID", &id, ImGuiInputTextFlags_ReadOnly);
        }
    }
    
    void MenuStage::onBecomeTop()
    {
        m_items.clear();
        m_lastLevelUnlocked = m_game.getSave().lastLevelUnlocked;
        
        const glm::vec2 winSize = m_game.getWindow().getSize();
    
        // Each item is fixed size
        const glm::vec2 itemSize{128.0f};
    
        // To have empty space between the items and be more spaced,
        // We add padding between the items. Exprimed in pixel.
        // All computations are done with itemSize but the rendered size is (itemSize - padding).
        const glm::vec2 padding {10.0f, 20.0f};
    
        // Margin between the borders of the window and the border items
        // Since padding is computed twice,
        // We need to compensate (and we can also give a different value for a style)
        const float margin = 32.0f;
    
        // Drawable area in center
        // Count twice margin (left and right)
        const float viewWidth { winSize.x - margin * 2.0f };
    
        // We search how many rects we can fill
        m_columns = std::floor(viewWidth / itemSize.x); // Columns count (integer)
        m_rows = std::ceil(static_cast<float>(m_lastLevelUnlocked) / static_cast<float>(m_columns));
        
        // Keep the focused button if there was one
        const GridPosition previousFocus{m_focus};
        
        m_focus = GridPosition{m_columns};
        m_focus.setPosition(*previousFocus);
        
        const float columnWidth = viewWidth / static_cast<float>(m_columns);
    
        GridPosition gridPos(m_columns);
    
        for(int i = 0; i < static_cast<int>(m_lastLevelUnlocked); ++i)
        {
            const auto item = std::make_shared<Item>(itemSize - padding, m_game.getFont(), i);
        
            float x = columnWidth * gridPos->x + columnWidth / 2.0f + margin;
            float y = winSize.y - margin - (itemSize.y * gridPos->y + itemSize.y / 2.0f);
        
            item->setPosition({x, y});
        
            m_items.push_back(item);
        
            ++gridPos;
        }
    
        m_items.at(m_focus.getIndex())->setFocused(true);
    }
    
    MenuStage::Item::Item(glm::vec2 size, const Font& font, int id)
        : Sprite(size)
    {
        setOrigin(Sprite::Centered);
        setFocused(false); // For setting default style
        
        m_label.setFont(&font);
        m_label.setString(cat(id + 1)); // IDs starts at 0, but label displayed starts at 1
        m_label.setPosition(size / 2.0f);
        m_label.setOrigin(Text::Centered);
        
    }
    
    void MenuStage::Item::draw(RenderStates states) const
    {
        Shape::draw(states);
        
        auto child = states;
        child.model *= getTransform();
        
        m_label.draw(child);
    }
    
    void MenuStage::Item::setFocused(bool focus)
    {
        if(focus)
        {
            setOutlineColor({1.0f, 0.0f, 1.0f, 0.5});
            setColor({1.0f, 0.0f, 1.0f, 0.75f});
        }
        else
        {
            setOutlineColor({1.0f, 0.0f, 1.0f, 0.5});
            setColor({1.0f, 1.0f, 1.0f, 0.5f});
        }
    }
}