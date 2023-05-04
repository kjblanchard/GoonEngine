#pragma once
#include <SDL_events.h>
#include <entt/fwd.hpp>
//TODO should we forward declare this?  Prolly doesn't matter and makes us use unique ptr easily
#include <Supergoon/panels/MenuBarPanel.hpp>
#include <Supergoon/panels/HierarchyPanel.hpp>
#include <Supergoon/panels/InspectorPanel.hpp>

namespace goon
{
    class Scene;
    class EditorLayer
    {
    public:
        void InitializeImGui();
        void ProcessImGuiEvents(SDL_Event *);
        void ImGuiNewFrame();
        void ProcessImGuiFrame(Scene &scene);
        void DrawImGuiFrame();
        void ExitImGui();
        void LoadScene(Scene &scene) { _scene = &scene; }
        entt::entity entitySelected;
        bool showDemoWindow = true;

    private:
        void CreateAllPanels();
        void DrawDebugWindow();


        private:
        std::unique_ptr<MenuBarPanel> _menuBarPanel;
        std::unique_ptr<HierarchyPanel> _hierarchyPanel;
        std::unique_ptr<InspectorPanel> _inspectorPanel;


        Scene *_scene;
    };
}