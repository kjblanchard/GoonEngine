#include <Supergoon/panels/InspectorPanel.hpp>
#include <Supergoon/layers/EditorLayer.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/BgmComponent.hpp>
#include <Goon/scene/components/IdComponent.hpp>
#include <Goon/scene/components/HierarchyComponent.hpp>
#include <Goon/systems/SoundSystem.hpp>
#include <entt/entt.hpp>
#include <imgui.h>

namespace goon
{
    InspectorPanel::InspectorPanel(EditorLayer* editor)
    : Panel(), _editor(editor)
    {
    }

    void InspectorPanel::DrawPanel(Scene *scene)
    {
        ImGui::Begin("Inspector");
        static bool updatingName = false;
        static char editNameBuffer[128];
        if (_editor->entitySelected != entt::null)
        {
            goon::GameObject gameobject{(entt::entity)_editor->entitySelected, scene};
            auto name = gameobject.GetComponent<goon::TagComponent>().Tag.c_str();

            ImGui::Text("Name: %s", name);
            ImGui::SameLine();
            if (ImGui::Button("Edit Name"))
            {
                updatingName = !updatingName;
                snprintf(editNameBuffer, sizeof(editNameBuffer), "%s", name);
            }
            if (updatingName)
            {
                ImGui::InputText("Name", editNameBuffer, IM_ARRAYSIZE(editNameBuffer));
                ImGui::SameLine();
                if (ImGui::Button("Update"))
                {
                    gameobject.GetComponent<goon::TagComponent>().Tag = std::string(editNameBuffer);
                    updatingName = !updatingName;
                }
            }
            bool hasbgm = gameobject.HasComponent<goon::BgmComponent>();
            bool hasHierarchy = gameobject.HasComponent<goon::HierarchyComponent>();
            bool hasId = gameobject.HasComponent<goon::IdComponent>();
            if (hasbgm)
            {
                if (ImGui::TreeNode((void *)(intptr_t)gameobject.GetID(), "Bgm Component "))
                {
                    // If we didn't remove the component with the popup.., this needs better
                    if (!RemoveComponentPopup<goon::BgmComponent>(scene, (entt::entity)gameobject.GetID()))
                    {
                        auto &bgmComponent = gameobject.GetComponent<goon::BgmComponent>();
                        ImGui::Text("Filename:");
                        ImGui::SameLine();
                        char buffer[256];
                        memset(buffer, 0, sizeof(buffer));
                        snprintf(buffer, sizeof(buffer), "%s", bgmComponent.SoundFile.c_str());
                        if (ImGui::InputText("##Filename", buffer, sizeof(buffer)))
                        {

                            bgmComponent.UpdateSoundFileName(std::string(buffer));
                        }
                        ImGui::Text("Begin Point (s):");
                        ImGui::SameLine();
                        if (ImGui::InputFloat("##Begin", &bgmComponent.LoopBegin))
                        {
                            bgmComponent.UpdateLoopBegin(bgmComponent.LoopBegin);
                        }
                        ImGui::Text("End Point (s):");
                        ImGui::SameLine();
                        if (ImGui::InputFloat("##End", &bgmComponent.LoopEnd))
                        {
                            bgmComponent.UpdateLoopEnd(bgmComponent.LoopEnd);
                        }
                        ImGui::Text("Volume");
                        ImGui::SameLine();
                        ImGui::SliderFloat("##Volume", &bgmComponent.Volume, 0.0f, 1.0f);
                        if (ImGui::Button("Play"))
                        {
                            SSPlayBgm(bgmComponent.LoadedBgm.get()->_bgm, bgmComponent.Volume);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Pause"))
                        {
                            SSPauseBgm(bgmComponent.LoadedBgm.get()->_bgm);
                        }
                    }
                    ImGui::TreePop();
                }
            }
            if (hasHierarchy)
            {
                auto uniqueInt = _editor->GetComponentUniqueIntImGui<goon::HierarchyComponent>(&gameobject);
                if (ImGui::TreeNode(uniqueInt, "HierarchyComponent"))
                {
                    auto &hierarchyComponent = gameobject.GetComponent<goon::HierarchyComponent>();
                    ImGui::Text("Parent: %d", hierarchyComponent.Parent);
                    ImGui::Text("NextChild: %d", hierarchyComponent.NextChild);
                    ImGui::Text("PrevChild: %d", hierarchyComponent.PreviousChild);
                    ImGui::Text("FirstChild: %d", hierarchyComponent.FirstChild);
                    ImGui::TreePop();
                }
            }
            if (hasId)
            {
                auto uniqueInt = _editor->GetComponentUniqueIntImGui<goon::IdComponent>(&gameobject);
                if (ImGui::TreeNode(uniqueInt, "IdInfo"))
                {
                    auto &idComponent = gameobject.GetComponent<goon::IdComponent>();
                    ImGui::Text("EntityId: %lld", gameobject.GetID());
                    uint64_t guid = idComponent.Guid;
                    ImGui::Text("Guid: %llu", guid);
                    ImGui::TreePop();
                }
            }
            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");
            if (ImGui::BeginPopup("AddComponent"))
            {
                ImGui::Text("Component Name");
                ImGui::Separator();
                if (!hasbgm)
                {
                    if (ImGui::Selectable("Bgm"))
                        gameobject.AddComponent<goon::BgmComponent, std::string, float, float, bool, float>("none", 0, 0, false, 0);
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    template <typename T>
    bool InspectorPanel::RemoveComponentPopup(Scene *scene, entt::entity entityRightClicked)
    {
        bool removedComponent = false;
        if (ImGui::BeginPopupContextItem(nullptr)) // entityTag is used as a id for the popup
        {
            if (ImGui::Button("Remove Component"))
            {
                goon::GameObject go{entityRightClicked, scene};
                go.RemoveComponent<T>();
                ImGui::CloseCurrentPopup();
                removedComponent = true;
            }
            ImGui::EndPopup();
        }
        return removedComponent;
    }
}