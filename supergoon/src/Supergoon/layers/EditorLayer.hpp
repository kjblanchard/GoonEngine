#pragma once
#include <SDL_events.h>
#include <entt/fwd.hpp>
namespace ImGui
{
    class ImVec2;
}

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
        entt::entity RecursiveDraw(entt::entity entity, std::vector<uint64_t> &parents);
        void CreateImGuiPopup(entt::entity entityRightClicked);
        template <typename T>
        bool RemoveComponentPopup(entt::entity entityRightClicked);
        void DragDropSource(entt::entity entity, std::string &entityName);
        void DragDropTargetAppend(entt::entity appendEntity, std::vector<uint64_t>& parents);
        void DragDropTargetBetween(entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents);
        entt::entity entitySelected = entt::null;
        bool lastFrameDrag = false;
        bool thisFrameDrag = false;
        bool showDemoWindow = true;

    private:
        void DrawMainMenu();
        void DrawDebugWindow();
        void DrawHierarchy();
        void DrawInspector();
        Scene *_scene;
    };
}