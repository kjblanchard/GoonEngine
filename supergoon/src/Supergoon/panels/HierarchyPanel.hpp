#pragma once
#include <Supergoon/panels/Panel.hpp>
#include <entt/fwd.hpp>

namespace goon
{
    class EditorLayer;
    class HierarchyPanel : public Panel
    {
    public:
        HierarchyPanel(EditorLayer *editor);
        ~HierarchyPanel() = default;
        virtual void DrawPanel(Scene *scene);
        bool lastFrameDrag = false;
        bool thisFrameDrag = false;
        void CreateImGuiPopup(Scene *scene, entt::entity entityRightClicked);
        void DragDropTargetAppend(Scene *scene, entt::entity appendEntity, std::vector<uint64_t> &parents);
        void DragDropTargetBetween(Scene *scene, entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents);
        entt::entity RecursiveDraw(Scene *scene, entt::entity entity, std::vector<uint64_t> &parents);
        void DragDropSource(entt::entity entity, std::string &entityName);

        EditorLayer *_editor;
    };
}