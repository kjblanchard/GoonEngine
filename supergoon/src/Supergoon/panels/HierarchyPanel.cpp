#include <Supergoon/panels/HierarchyPanel.hpp>
#include <Goon/scene/Scene.hpp>
#include <Goon/scene/components/HierarchyComponent.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/InactiveComponent.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Supergoon/layers/EditorLayer.hpp>
#include <imgui.h>

namespace goon
{
    HierarchyPanel::HierarchyPanel(EditorLayer *editor)
        : Panel(), _editor(editor)
    {
    }
    void HierarchyPanel::DrawPanel(Scene *scene)
    {
        ImGui::Begin("Hierarchy");
        // Handle dragging bool
        lastFrameDrag = thisFrameDrag;
        if (ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            thisFrameDrag = true;
        else
            thisFrameDrag = false;

        // Draw root, and then all others
        goon::GameObject rootGo{scene->RootObject, scene};
        auto &rootHierarchy = rootGo.GetComponent<goon::HierarchyComponent>();
        entt::entity currentDrawingEntity = rootHierarchy.FirstChild;
        std::vector<uint64_t> parents;
        if (ImGui::TreeNode(scene->SceneName().c_str()))
        {
            CreateImGuiPopup(scene, rootGo);
            DragDropTargetAppend(scene, scene->RootObject, parents);
            DragDropTargetBetween(scene, entt::null, rootGo, parents);

            while (currentDrawingEntity != entt::null)
            {
                currentDrawingEntity = RecursiveDraw(scene, currentDrawingEntity, parents);
                parents.clear();
            }
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void HierarchyPanel::CreateImGuiPopup(Scene *scene, entt::entity entityRightClicked)
    {
        if (ImGui::BeginPopupContextItem(nullptr)) // entityTag is used as a id for the popup
        {
            _editor->entitySelected = entityRightClicked;
            std::string name = "Unnamed";

            if (ImGui::Button("Create Gameobject at root"))
            {
                scene->CreateGameObject(name, scene->RootObject);
                ImGui::CloseCurrentPopup();
            }
            if (scene->RootObject != entityRightClicked)
            {
                if (ImGui::Button("Create child Gameobject"))
                {
                    scene->CreateGameObject(name, entityRightClicked);
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Delete GameObject"))
                {
                    auto gameobject = goon::GameObject{entityRightClicked, scene};
                    auto hierarchy = gameobject.GetComponent<goon::HierarchyComponent>();
                    auto parentGameobject = goon::GameObject{hierarchy.Parent, scene};
                    parentGameobject.RemoveChildEntity(entityRightClicked);
                    scene->DestroyGameObject((uint64_t)_editor->entitySelected);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }
    void HierarchyPanel::DragDropTargetAppend(Scene *scene, entt::entity appendEntity, std::vector<uint64_t> &parents)
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("test"))
            {
                auto appendGO = goon::GameObject{appendEntity, scene};
                IM_ASSERT(payload->DataSize == sizeof(uint64_t));
                uint64_t droppedEntityId = *(const uint64_t *)payload->Data;
                goon::GameObject droppedGameObject{(entt::entity)droppedEntityId, scene};
                if (std::find(parents.begin(), parents.end(), droppedGameObject.GetID()) == parents.end())
                {
                    auto &hierarchy = droppedGameObject.GetComponent<goon::HierarchyComponent>();
                    auto oldParentGameObject = goon::GameObject{hierarchy.Parent, scene};
                    oldParentGameObject.RemoveChildEntity((entt::entity)droppedGameObject);
                    appendGO.AppendChildEntity((entt::entity)droppedEntityId);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    void HierarchyPanel::DragDropTargetBetween(Scene *scene, entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents)
    {
        if (!(thisFrameDrag || lastFrameDrag))
            return;
        ImGui::Dummy(ImVec2{200, 5});
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("test"))
            {
                IM_ASSERT(payload->DataSize == sizeof(uint64_t));
                uint64_t droppedEntityId = *(const uint64_t *)payload->Data;
                goon::GameObject droppedGameObject{(entt::entity)droppedEntityId, scene};
                if (previousChild != droppedGameObject)
                {
                    if (std::find(parents.begin(), parents.end(), droppedGameObject.GetID()) == parents.end())
                    {
                        auto &hierarchy = droppedGameObject.GetComponent<goon::HierarchyComponent>();
                        auto oldParentGameObject = goon::GameObject{hierarchy.Parent, scene};
                        oldParentGameObject.RemoveChildEntity((entt::entity)droppedGameObject);
                        auto newParentGameObject = goon::GameObject{parent, scene};
                        newParentGameObject.AddChildEntity((entt::entity)droppedGameObject.GetID(), previousChild);
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
    entt::entity HierarchyPanel::RecursiveDraw(Scene *scene, entt::entity entity, std::vector<uint64_t> &parents)
    {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        goon::GameObject gameobject{entity, scene};
        auto &hierarchyComponent = gameobject.GetComponent<goon::HierarchyComponent>();
        auto &tagComponent = gameobject.GetComponent<goon::TagComponent>();
        bool isInactive = gameobject.HasComponent<goon::InactiveComponent>();
        if (isInactive) // Try next child if we are inactive, this breaks out early.
            return hierarchyComponent.NextChild;
        if (hierarchyComponent.FirstChild == entt::null) // If we have no children, we draw a selectable.
        {
            ImGui::PushID(_editor->GetGameobjectUniqueIntImgui(&gameobject));
            if (ImGui::Selectable(tagComponent, gameobject.GetEntity() == _editor->entitySelected))
            {
                _editor->entitySelected = gameobject.GetEntity();
            }
            DragDropSource(entity, tagComponent.Tag);
            DragDropTargetAppend(scene, entity, parents);
            ImGui::PopID();
            CreateImGuiPopup(scene, entity);
            DragDropTargetBetween(scene, entity, hierarchyComponent.Parent, parents);
        }
        else // If we do have children
        {
            auto node_flags = base_flags;
            if (_editor->entitySelected == gameobject.GetEntity())
            {
                node_flags |= 1 << 0;
            }
            bool node_open = ImGui::TreeNodeEx(_editor->GetGameobjectUniqueIntImgui(&gameobject), node_flags, "%s", tagComponent.Tag.c_str());
            DragDropSource(entity, tagComponent.Tag);
            DragDropTargetAppend(scene, entity, parents);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                _editor->entitySelected = gameobject.GetEntity();
            CreateImGuiPopup(scene, entity);
            if (node_open)
            {
                auto newParents = std::vector<uint64_t>(parents);
                newParents.push_back(gameobject.GetID());
                DragDropTargetBetween(scene, entt::null, entity, newParents);
                auto nextChild = hierarchyComponent.FirstChild;
                while (nextChild != entt::null)
                {
                    nextChild = RecursiveDraw(scene, nextChild, newParents);
                    std::remove(newParents.begin(), newParents.end(), gameobject.GetID());
                }
                ImGui::TreePop();
            }
            DragDropTargetBetween(scene, entity, hierarchyComponent.Parent, parents);
        }
        return hierarchyComponent.NextChild;
    }

    void HierarchyPanel::DragDropSource(entt::entity entity, std::string &entityName)
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("test", &entity, sizeof(uint64_t));
            // Display preview (could be anything, e.g. when dragging an image we could decide to display
            // the filename and a small preview of the image, etc.)
            ImGui::Text("Move %s", entityName.c_str());
            ImGui::EndDragDropSource();
        }
    }

}
