#pragma once

#include "media/scene/SceneStack.hpp"
#include "media/AudioEngine.hpp"
#include "GameControls.hpp"
#include "media/TextureCache.hpp"
#include "gameplay/Stage.hpp"
#include "gameplay/Save.hpp"
#include "media/Window.hpp"
#include "media/AudioEngine.hpp"
#include <wrappers/gl/Shader.hpp>
#include <wrappers/freetype/Font.hpp>
#include <wrappers/box2d/DebugDraw.hpp>
#include <wrappers/box2d/World.hpp>
#include <utility/Rect.hpp>
#include <box2d/box2d.h>

namespace SpaceNinja
{

class Game
{
public:
    Game();

    /// @brief Game loop.
    /// @details Only terminate when the game terminates.
    void show();

    void draw();

    void update();

    Snow::media::Window &getWindow();

    const Snow::media::Window &getWindow() const;

    std::filesystem::path getRoot() const;

    const Shader &getShader() const;

    Shader &getShader();

    /// @brief Get the default global font
    Font &getFont();

    const Font &getFont() const;

    /// @brief Start or re-pushSceneStage the level with this id (starts at 0).
    void pushSceneStage(int id);

    // !!!Order of member variable is important: for example we need OpenGL context before almost everything

    Snow::media::Window m_window;

    const GameControls &getControls() const;

    /// @brief Returns to the main menu by adding a new process.
    /// @remarks Do not remove other processes, so they should be removed, for example to avoid
    /// two menus at the same time, a stage running during the menu, etc.
    void showMainMenu();


    /// @brief Save to a file storage the current Save.
    /// @details
    ///     The Save lives only in memory, so it has to be called to be taken into consideration for next
    ///     execution.
    void writeSave() const;

public:
    Snow::media::TextureCache textures;
    SceneStack scene;

    Save &getSave() { return *m_save; }

    const Save &getSave() const { return *m_save; }

    Snow::media::AudioEngine &getAudio() { return m_audio; }

    float getAspectRatio() const { return m_aspectRatio; }

private:
    /// @brief Calls to glViewport() before every frame.
    /// @details
    ///     The game is designed in 16:9.
    ///         - When the window is 16:9, the content is zoomed
    ///         - When the window is not 16:9, there are black borders in one dimension, depending which one is the largest.
    void applyAspectRatio();

    void debug();

    std::shared_ptr<Save> m_save;
    Shader m_shader;
    Font m_font;

    std::shared_ptr<GameControls> m_userControls;


    /// @brief Node for rendering debug informations, separate from the Scene (to be always updated)
    SceneNode m_debugNode;

    Snow::media::AudioEngine m_audio;

    bool m_showWindowBorders{false};
    float m_aspectRatio{16.0f / 9.0f};
};

}