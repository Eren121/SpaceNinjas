#include "Game.hpp"
#include "media/WindowDebugInfo.hpp"
#include "media/Window.hpp"
#include "process/CoProcess.hpp"
#include "ui/VerticalListMenu.hpp"
#include "ui/MenuStage.hpp"
#include <imgui.h>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace sn
{
    Game::Game()
        : m_window("Haaa", 800, 600),
          textures(getRoot() / "textures"),
          sheets(getRoot() / "textures"),
          m_save(std::make_shared<Save>())
    {
        GL::enableDebugging();

        {
            const std::filesystem::path savePath{getRoot() / "save.json"};

            if (!std::filesystem::exists(savePath)) {
                // Create default one
                // To check it was successfully constructed, also read it again even if it's not-necessary

                std::ofstream jsonOut{savePath};
                cereal::JSONOutputArchive ar{jsonOut};
                ar(*m_save);
            }

            std::ifstream jsonIn{savePath};
            cereal::JSONInputArchive ar{jsonIn};
            ar(*m_save);
        }

        std::filesystem::path assets = std::filesystem::current_path() / ".." / "assets";
        m_shader.load(assets / "vert.glsl", assets / "frag.glsl");

        auto fontPath = getRoot() / "fonts" / "monofonto.ttf";
        m_font = Font(fontPath);

        SDL::printVersion("FreeType", Font::getFreetypeCompiledVersion(), m_font.getFreetypeLinkedVersion());

        m_userControls = std::make_shared<GameControls>(m_window.getInput());

        m_debugNode.addChild(m_save);
        m_debugNode.addChild(m_userControls);;
        m_debugNode.addChild(std::make_shared<Snow::media::WindowDebugInfo>(m_window));

        textures.forceLoad("@missing", [](Texture& texture) {
            texture.loadMissingTexture();
        });
    }

    void Game::showMainMenu()
    {
        auto menu = std::make_shared<ui::VerticalListMenu>(*this);

        menu->addOption("Play", [this](SceneNode&) {
            scene.push(std::make_shared<ui::MenuStage>(*this));
        });

        menu->addOption("Settings");

        menu->addOption("Quit", [this](SceneNode&) {
            m_window.close();
        });

        scene.push(menu);
    }

    void Game::show()
    {
        showMainMenu();

        while (m_window.isOpen())
        {

            // Handle window resizing
            applyAspectRatio();

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

            const glm::vec4 clearColor{0.0f, 0.0f, 0.0f, 1.0f};

            if(m_showWindowBorders)
            {
                const glm::vec4 bordersColor{1.0f, 0.0f, 1.0f, 1.0f};

                // Test pour mettre en évidence les bordures
                // glClear() n'est pas affecté par glViewport() donc on utilise glScissors qui prend les mêmes arguments

                glClearColor(glVec4(bordersColor));
                glClear(GL_COLOR_BUFFER_BIT);

                glEnable(GL_SCISSOR_TEST);
                {
                    glClearColor(glVec4(clearColor));
                    glClear(GL_COLOR_BUFFER_BIT);
                }
                glDisable(GL_SCISSOR_TEST);
            }
            else
            {
                glClearColor(glVec4(clearColor));
                glClear(GL_COLOR_BUFFER_BIT);
            }

            glClear(GL_DEPTH_BUFFER_BIT);

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
        if (m_userControls->showDebugWindow.isJustPressed())
        {
            printDebugInfo = !printDebugInfo;
        }

        if(printDebugInfo)
        {
            ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoBackground);
            debug();
        }

        m_audio.update();

        scene.update(printDebugInfo);
        m_debugNode.update(printDebugInfo);

        if(printDebugInfo)
        {
            ImGui::End();
        }

        if(getControls().switchFullscreen.isJustPressed())
        {
            using Snow::media::Window;

            // Toggle fake fullscreen / windowed

            if(m_window.getFullscreenState() == Window::Windowed)
            {
                m_window.setFullscreenState(Window::BorderlessFullscreen);
            }
            else
            {
                m_window.setFullscreenState(Window::Windowed);
            }
        }
    }

    std::filesystem::path Game::getRoot() const
    {
        auto root = std::filesystem::current_path() / ".." / "assets";
        return root;
    }

    Snow::media::Window &Game::getWindow()
    {
        return m_window;
    }

    const Snow::media::Window &Game::getWindow() const
    {
        return m_window;
    }

    Font &Game::getFont()
    {
        return m_font;
    }

    const Font &Game::getFont() const
    {
        return m_font;
    }

    const Shader &Game::getShader() const
    {
        return m_shader;
    }

    Shader &Game::getShader()
    {
        return m_shader;
    }

    const GameControls &Game::getControls() const
    {
        return *m_userControls;
    }

    void Game::writeSave() const
    {
        const std::filesystem::path savePath{getRoot() / "save.json"};

        if (!std::filesystem::exists(savePath)) {
            std::cerr << "Creating a new save file " << savePath << "..." << std::endl;
        } else {
            std::cerr << "Overwriting save file " << savePath << "..." << std::endl;
        }

        std::ofstream jsonOut{savePath};
        cereal::JSONOutputArchive ar{jsonOut};
        ar(*m_save);
    }

    void Game::applyAspectRatio()
    {
        const glm::vec2 screenSize{m_window.getSize()};
        const float aspectRatio{m_aspectRatio};

        glm::vec2 viewportSize;

        if(screenSize.x / screenSize.y > aspectRatio)
        {
            // Width is bigger in proportion than in 16:9
            // We take the height as reference
            // Black borders at top and bottom
            // Most probably case in fullscreen for most common alternatives ratios, 4:3 or 16:10

            viewportSize.y = screenSize.y;
            viewportSize.x = screenSize.y * aspectRatio;
        }
        else
        {
            // Height is bigger in proportion than in 16:9
            // We take width as reference
            // Black borders at left and right

            viewportSize.x = screenSize.x;
            viewportSize.y = screenSize.x / aspectRatio;
        }

        // Centerize
        // Origin is bottom-left

        glm::vec2 origin;
        origin.x = (screenSize.x - viewportSize.x) / 2.0f;
        origin.y = (screenSize.y - viewportSize.y) / 2.0f;

        glViewport(glVec2(origin), glVec2(viewportSize));
        glScissor(glVec2(origin), glVec2(viewportSize)); // To test to draw black borders
    }

    void Game::debug()
    {
        if(ImGui::CollapsingHeader("General"))
        {
            {
                // To explore ImGui possibilities, integrate a demo window
                static bool showDemoWindow{false};
                ImGui::Checkbox("Show ImGUI demo window", &showDemoWindow);
                if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); }
            }
            {
                using Snow::media::Window;

                // Window fullscreen fs_state
                static int fs_state{Window::Windowed};

                // pressed == if any radio is newly pressed this frame
                bool pressed{false};
                pressed |= ImGui::RadioButton("Windowed", &fs_state, Window::Windowed); ImGui::SameLine();
                pressed |= ImGui::RadioButton("True Fullscreen", &fs_state, Window::Fullscreen); ImGui::SameLine();
                pressed |= ImGui::RadioButton("Fake fullscreen", &fs_state, Window::BorderlessFullscreen);

                if(pressed)
                {
                    m_window.setFullscreenState(static_cast<Window::FullscreenState>(fs_state));
                }
            }

            ImGui::Checkbox("Show window borders", &m_showWindowBorders);
        }
    }

    glm::mat4 Game::getUIViewMatrix() const
    {
        const glm::vec2 uiSize{getUISize()};
        return glm::ortho(0.0f, uiSize.x, 0.0f, uiSize.y);
    }

    glm::vec2 Game::getUISize() const
    {
        return {1920.0f, 1080.0f};
    }
}