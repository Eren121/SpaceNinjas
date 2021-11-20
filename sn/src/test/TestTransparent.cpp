#include "test/TestTransparent.hpp"
#include <gl/VertexArray.hpp>
#include <gl/ConvexShape.hpp>
#include <gl/Line.hpp>
#include <math.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <glm/gtc/matrix_transform.hpp>

namespace sn::test
{

TestTransparent::TestTransparent()
        : m_window("Test transparent", 800, 800)
{
    std::filesystem::path path = std::filesystem::current_path() / "../assets";

    m_shader.load(path / "vert.glsl", path / "frag.glsl");
}

void TestTransparent::draw()
{
    RenderStates states;
    states.shader = &m_shader;


    Sprite a, b;

    a.setPosition({-0.75f, -0.75f});
    b.setPosition({-0.25f, -0.25f});
    a.setColor({1.0f, 0.0f, 0.0f, 0.5f});
    b.setColor({0.0f, 1.0f, 0.0f, 0.6f});

    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

    a.draw(states);
    b.draw(states);
}

void TestTransparent::run()
{
    // https://decovar.dev/blog/2019/05/26/sdl-imgui/#sdl

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui_ImplSDL2_InitForOpenGL(m_window.getHandle(), nullptr);

    sigslot::scoped_connection conn = m_window.onEvent.connect([](const SDL_Event &event, long) {
        ImGui_ImplSDL2_ProcessEvent(&event);
    });

    while (m_window.isOpen()) {
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplSDL2_NewFrame();
//        ImGui::NewFrame();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        // GL_LEQUAL explanation:
        // If we want the depth test enabled, we have to draw if the new depth is <= than the old depth,
        // Because we probably will have all the vertices on the same plane in 2D (0 in Z)
        // We could also just disable DEPTH_TEST, but maybe we will need it for further use
        // (default if GL_LESS)

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.0f, 0.0f, 0.0f, .2f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_window.handleEvents();

        draw();

//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window.display();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    exit(0);
}

}