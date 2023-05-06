#include <Supergoon/layers/EditorLayer.hpp>
#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <Goon/scene/Scene.hpp>
#include <Goon/core/Application.hpp>

#define IMGUI_DEMO_GOON

namespace goon
{
    void *EditorLayer::GetGameobjectUniqueIntImgui(GameObject *gameobject)
    {
        return (void *)(intptr_t)(gameobject->GetID() + typeid(GameObject).hash_code());
    }

    void EditorLayer::InitializeImGui()
    {
        ImGui::CreateContext();
        auto application = Application::GetApplication();
        IMGUI_CHECKVERSION();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        ImGui_ImplOpenGL3_Init(application->glslVersion);
        bool imGuiDarkMode = false;
        if (imGuiDarkMode)
            ImGui::StyleColorsDark();
        else
            ImGui::StyleColorsLight();
        ImGui_ImplSDL2_InitForOpenGL(application->GetWindow(), application->GetGLContext());
        CreateAllPanels();
    }
    void EditorLayer::CreateAllPanels()
    {
        _menuBarPanel = std::make_unique<MenuBarPanel>();
        _hierarchyPanel = std::make_unique<HierarchyPanel>(this);
        _inspectorPanel = std::make_unique<InspectorPanel>(this);
    }

    void EditorLayer::ProcessImGuiEvents(SDL_Event *event)
    {
        ImGui_ImplSDL2_ProcessEvent(event);
    }
    void EditorLayer::ImGuiNewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void EditorLayer::ProcessImGuiFrame()
    {
        if (!_scene)
            return;
        _menuBarPanel->DrawPanel(_scene);
        _hierarchyPanel->DrawPanel(_scene);
        _inspectorPanel->DrawPanel(_scene);
        DrawDebugWindow();
        ImGui::Render();
    }

    void EditorLayer::DrawDebugWindow()
    {
#ifdef IMGUI_DEMO_GOON
        bool boy = true;
        ImGui::ShowDemoWindow(&boy);
#endif
    }

    void EditorLayer::DrawImGuiFrame()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        auto &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }

    void EditorLayer::ExitImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

}
