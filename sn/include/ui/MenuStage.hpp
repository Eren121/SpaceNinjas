#pragma once

#include <ft/Text.hpp>
#include <gl/Sprite.hpp>
#include <media/scene/SceneNode.hpp>
#include <process/CoProcess.hpp>
#include <GridPosition.hpp>
#include <logging.hpp>
#include "Game.hpp"
#include "Fwd.hpp"

namespace sn::ui
{
    /// @brief The menu to choose a Stage among the unlocked Stages.
    class MenuStage : public SceneNode, private Snow::exe::CoProcess
    {
        LOGGABLE("MenuStage")

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

        /// @brief The last row may be incomplete if nb_levels % nb_cols != 0
        int getColumnsCountOnLastRow() const;
        
    public:
        /// @param stages The vector of all stage to render. true means the stage is unlocked.
        explicit MenuStage(sn::Game& game);

        void onBecomeTop() override;
        
    protected:
        void drawNode(RenderStates states) const override;
        void debugNode() override;
        bool updateNode() override;
    
        task<> coroutine() override;
        
    private:
        sn::Game& m_game;

        int m_cols{0};
        int m_rows{0};
        int m_lastLevelUnlocked{0};
        
        GridPosition m_focus;
        std::vector<std::shared_ptr<Item>> m_items;
    };
}