#include <Supergoon/layers/EditorLayer.hpp>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <Goon/scene/Scene.hpp>
#include <Goon/core/Application.hpp>

namespace goon
{

    void EditorLayer::InitializeImGui()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        ImGui::StyleColorsLight();
        auto application = Application::GetApplication();
        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForSDLRenderer(application->GetWindow(), application->GetRenderer());
        ImGui_ImplSDLRenderer_Init(application->GetRenderer());
        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        // IM_ASSERT(font != NULL);

        //TODO this should be moved to an Init function
        CreateAllPanels();
    }
        void EditorLayer::CreateAllPanels()
        {
            //TODO shared ptr
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
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void EditorLayer::ProcessImGuiFrame(Scene &scene)
    {
        _menuBarPanel->DrawPanel(&scene);
        _hierarchyPanel->DrawPanel(&scene);
        _inspectorPanel->DrawPanel(&scene);
        DrawDebugWindow();
        ImGui::Render();
    }

    void EditorLayer::DrawDebugWindow()
    {
        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);
    }

    void EditorLayer::DrawImGuiFrame()
    {
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }

    void EditorLayer::ExitImGui()
    {
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }


}
