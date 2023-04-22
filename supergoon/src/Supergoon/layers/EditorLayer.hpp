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
        // Statics moved here
        entt::entity RecursiveDraw(entt::entity entity, goon::Scene &scene, std::vector<uint64_t> &parents);
        void CreateImGuiPopup(goon::Scene &scene, entt::entity entityRightClicked);
        template <typename T>
        bool RemoveComponentPopup(goon::Scene &scene, entt::entity entityRightClicked);
        void DragDropSource(entt::entity entity, std::string &entityName);
        void DragDropTargetAppend(entt::entity appendEntity, goon::Scene &scene);
        void DragDropTargetBetween(entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents, goon::Scene &scene);

        int entitySelected = -1;
        bool lastFrameDrag = false;
        bool thisFrameDrag = false;
        bool showDemoWindow = true;
    };
}