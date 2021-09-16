#pragma once

#include "media/scene/SceneStack.hpp"
#include "ViewControl.hpp"
#include "GameControls.hpp"
#include "TextureCache.hpp"
#include "gameplay/Stage.hpp"
#include "gameplay/Save.hpp"
#include "media/Window.hpp"
#include <wrappers/gl/Shader.hpp>
#include <wrappers/freetype/Font.hpp>
#include <wrappers/box2d/DebugDraw.hpp>
#include <wrappers/box2d/World.hpp>
#include <utility/Rect.hpp>
#include <box2d/box2d.h>

class Game
{
public:
    Game();

    /// @brief Game loop.
    /// @details Only terminate when the game terminates.
    void show();
    void draw();
    void update();

    Window& getWindow();
    const Window& getWindow() const;

    std::filesystem::path getRoot() const;
    
    const Shader& getShader() const;
    Shader& getShader();

    /// @brief Build the view matrix from the view Rect.
    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    /// @brief Get the default global font
    Font& getFont();
    const Font& getFont() const;

    /// @brief Start or re-pushSceneStage the level with this id (starts at 0).
    void pushSceneStage(int id);

    // !!!Order of member variable is important: for example we need OpenGL context before almost everything

    Window m_window;
    
    float getPixelPerMeter() const;

    const GameControls& getControls() const;
    
    /// @brief Returns to the main menu by adding a new process.
    /// @remarks Do not remove other processes, so they should be removed, for example to avoid
    /// two menus at the same time, a stage running during the menu, etc.
    void showMainMenu();
    
public:
    TextureCache textures;
    SceneStack scene;
    Save save;
    
private:
    Shader m_shader;
    Font m_font;
    
    std::shared_ptr<GameControls> m_userControls;
    
    
    /// @brief Node for rendering debug informations, separate from the Scene (to be always updated)
    SceneNode m_debugNode;
};
