#pragma once

#include <wrappers/freetype/Text.hpp>
#include "media/scene/SceneNode.hpp"
#include "process/CoProcess.hpp"
#include "wrappers/gl/Sprite.hpp"
#include "utility/GridPosition.hpp"

class Game;

namespace ui
{
    /// @brief The menu to choose a Stage among the unlocked Stages.
    class MenuStage : public SceneNode, private CoProcess
    {
    public:
        /// @brief VerticalListMenu Stage item
        class Item : public Sprite
        {
        public:
            explicit Item(glm::vec2 size, const Font& font, int id);
            
            void draw(RenderStates states) const override;

            void setFocused(bool focus);
            
        private:
            Text m_label;
        };
        
    public:
        /// @param stages The vector of all stage to render. true means the stage is unlocked.
        explicit MenuStage(Game& game);

        void onBecomeTop() override;
        
    protected:
        void drawNode(RenderStates states) const override;
        void debugNode() override;
        bool updateNode() override;
    
        task<> coroutine() override;
        
    private:
        Game& m_game;
        
        /// @brief Count of items on each row
        int m_columns;
        int m_rows;
        int m_lastLevelUnlocked;
        
        GridPosition m_focus;
        std::vector<std::shared_ptr<Item>> m_items;
    };
}