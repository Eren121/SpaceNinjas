#include "Game.hpp"
#include "WindowDebugInfo.hpp"
#include "process/CoProcess.hpp"
#include "process/Wait.hpp"
#include "ui/MainMenu.hpp"
#include "ui/MenuStage.hpp"
#include <wrappers/gl/Circle.hpp>
#include <wrappers/gl/Shader.hpp>
#include <wrappers/gl/Texture.hpp>
#include <GL/glew.h>
#include <imgui.h>
#include <cereal/archives/json.hpp>
#include <fstream>

Game::Game()
    : m_window("Haaa", 800, 600),
      textures(getRoot() / "textures")
{
    GL::enableDebugging();

    {
        const std::filesystem::path savePath{getRoot() / "save.json"};
        
        if(!std::filesystem::exists(savePath))
        {
            // Create default one
            // To check it was successfully constructed, also read it again even if it's not-necessary
            
            std::ofstream jsonOut{savePath};
            cereal::JSONOutputArchive ar{jsonOut};
            save.serialize(ar);
        }
        
        std::ifstream jsonIn{savePath};
        cereal::JSONInputArchive ar{jsonIn};
        save.serialize(ar);
    }
    
    std::filesystem::path assets = std::filesystem::current_path() / "../assets";
    m_shader.load(assets / "vert.glsl", assets / "frag.glsl");

    auto fontPath = getRoot() / "fonts" / "monofonto.ttf";
    m_font = Font(fontPath);
    
    SDL::printVersion("FreeType", Font::getFreetypeCompiledVersion(), m_font.getFreetypeLinkedVersion());
    
    m_userControls = std::make_shared<GameControls>(m_window.getInput());
    
    m_debugNode.addChild(m_userControls);;
    m_debugNode.addChild(std::make_shared<WindowDebugInfo>(m_window));
}

void Game::showMainMenu()
{
    auto menu = std::make_shared<ui::MainMenu>(*this);
    
    menu->addOption("Play", [this] {
        std::vector<bool> stages(50);
        
        scene.push(std::make_shared<ui::MenuStage>(*this, stages));
    });
    
    menu->addOption("Settings");
    
    menu->addOption("Quit", [this] {
        m_window.close();
    });
    
    scene.push(menu);
}

void Game::show()
{
    showMainMenu();
    
    while(m_window.isOpen())
    {
        // We will need blending
        // Default blending
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glDepthFunc(GL_LEQUAL);
        // GL_LEQUAL explanation:
        // If we want the depth test enabled, we have to draw if the new depth is <= than the old depth,
        // Because we probably will have all the vertices on the same plane in 2D (0 in Z)
        // We could also just disable DEPTH_TEST, but maybe we will need it for further use
        // (default if GL_LESS)

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Handle window resizing
        glm::vec2 size = m_window.getSize();
        glViewport(0.0f, 0.0f, size.x, size.y);
        
        m_window.handleEvents();

        update();
        draw();
        
        m_window.display();
    }
}

void Game::pushSceneStage(int id)
{
    std::cout << "Starting level " << id << std::endl;
    
    std::shared_ptr<Stage> stage = std::make_shared<Stage>(*this, id);
    scene.push(stage);
}

void Game::draw()
{
    RenderStates states;
    states.shader = &m_shader;
    
    scene->draw(states);
    m_debugNode.draw(states);
}

void Game::update()
{
    static bool printDebugInfo{false};
    if(m_userControls->showDebugWindow.isJustPressed())
    {
        printDebugInfo = !printDebugInfo;
    }
    
    scene.update(printDebugInfo);
    m_debugNode.update(printDebugInfo);
}

std::filesystem::path Game::getRoot() const
{
    auto root = std::filesystem::current_path() / "../assets";
    return root;
}

Window& Game::getWindow()
{
    return m_window;
}

const Window& Game::getWindow() const
{
    return m_window;
}

Font& Game::getFont()
{
    return m_font;
}

const Font& Game::getFont() const
{
    return m_font;
}


glm::mat4 Game::getViewMatrix() const
{
    // m_view is in meters, we want in pixels
    // m => px
    // m * (px/m) = px

    const glm::vec2 windowSize = m_window.getSize();

    // Get the view size in meters (but based on the size of the window)
    Rect view(windowSize / getPixelPerMeter());
    view.setCenter({0.0f, 0.0f});

    // convert the view in [-1;1] to windowSize coordinates

    glm::mat4 topixel = glm::inverse(getProjectionMatrix());

    return topixel * glm::ortho(view.left(), view.right(), view.bottom(), view.top());
}

const Shader& Game::getShader() const
{
    return m_shader;
}

Shader& Game::getShader()
{
    return m_shader;
}

glm::mat4 Game::getProjectionMatrix() const
{
    glm::vec2 pixel = m_window.getSize();
    return glm::ortho(0.0f, pixel.x, 0.0f, pixel.y);
}

float Game::getPixelPerMeter() const
{
    return 30.0f;
}

const GameControls& Game::getControls() const
{
    return *m_userControls;
}
