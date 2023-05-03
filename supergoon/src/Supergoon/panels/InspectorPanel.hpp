#pragma once
#include <Supergoon/panels/Panel.hpp>
// #include <entt/fwd.hpp>
#include <entt/entt.hpp>

namespace goon
{
    class Scene;
    class EditorLayer;
    class InspectorPanel : public Panel
    {
    public:
        InspectorPanel(EditorLayer* editor);
        ~InspectorPanel() = default;
        virtual void DrawPanel(Scene *scene);
        EditorLayer *_editor;
        template <typename T>
        bool RemoveComponentPopup(Scene *scene, entt::entity entityRightClicked);
    };

}