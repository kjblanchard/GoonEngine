#pragma once
#include <SDL_events.h>
#include <entt/fwd.hpp>

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
        // Statics moved here
        entt::entity RecursiveDraw(entt::entity entity, std::vector<uint64_t> &parents);
        void CreateImGuiPopup(entt::entity entityRightClicked);
        template <typename T>
        bool RemoveComponentPopup(entt::entity entityRightClicked);
        void DragDropSource(entt::entity entity, std::string &entityName);
        void DragDropTargetAppend(entt::entity appendEntity);
        void DragDropTargetBetween(entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents);

        int entitySelected = -1;
        bool lastFrameDrag = false;
        bool thisFrameDrag = false;
        bool showDemoWindow = true;

    private:
        void DrawHierarchy();
        Scene *_scene;
    };
}