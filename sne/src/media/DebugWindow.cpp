#include "media/DebugWindow.hpp"
#include "im/ImGUI.hpp"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

DebugWindow::DebugWindow(SDL_Window *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
}

DebugWindow::~DebugWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void DebugWindow::onEvent(const SDL_Event& e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
}

void DebugWindow::onFrameStarted()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void DebugWindow::draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
