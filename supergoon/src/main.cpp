#include <iostream>
#include <memory>
#include <cstring>
#include <imgui.h>
#include <Goon/core/Application.hpp>
#include <Goon/scene/Scene.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Goon/core/bgm_asset.hpp>
#include <Goon/systems/SoundSystem.hpp>
#include <Goon/core/asset_manager.hpp>
#include <Goon/scene/components/BgmComponent.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/HierarchyComponent.hpp>
#include <Goon/scene/components/IdComponent.hpp>
#include <Goon/scene/components/InactiveComponent.hpp>
#include <Supergoon/commands/Action.hpp>
#include <Supergoon/layers/EditorLayer.hpp>

static void CreateImGuiPopup(goon::Scene &scene, entt::entity entityRightClicked);
static entt::entity RecursiveDraw(entt::entity entity, goon::Scene &scene, std::vector<uint64_t> &parents);
static void DragDropTargetBetween(entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents, goon::Scene &scene);
template <typename T>
static bool RemoveComponentPopup(goon::Scene &scene, entt::entity entityRightClicked);
static void DragDropTargetAppend(entt::entity appendEntity, goon::Scene &scene);
static void DragDropSource(entt::entity entity, std::string &entityName);

int demo(goon::Scene &scene);

static int entitySelected = -1;
bool lastFrameDrag = false;
bool thisFrameDrag = false;

int main(int argc, char **argv)
{
    goon::Scene scene;
    scene.DeSerializeScene();
    demo(scene);
}

int demo(goon::Scene &scene)
{
    auto application = goon::Application();
    application.InitializeSDL();
    auto editor = goon::EditorLayer();
    editor.InitializeImGui();


    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            editor.ProcessImGuiEvents(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(application.GetWindow()))
                done = true;
        }

        application.ResizeWindow();
// Start the Dear ImGui frame
        editor.ImGuiNewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "ctrl + s"))
                {
                    scene.SerializeScene();
                }
                ImGui::Text("Load");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::Text("Hierarchy");
                ImGui::Text("Inspector");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (editor.showDemoWindow)
            ImGui::ShowDemoWindow(&editor.showDemoWindow);

        ////
        // Hierarchy Panel
        ////
        ImGui::Begin("Hierarchy");
        // Handle dragging bool
        lastFrameDrag = thisFrameDrag;
        if (ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            thisFrameDrag = true;
        else
            thisFrameDrag = false;

        // Draw root, and then all others
        goon::GameObject rootGo{scene.RootObject, &scene};
        auto &rootHierarchy = rootGo.GetComponent<goon::HierarchyComponent>();
        entt::entity currentDrawingEntity = rootHierarchy.FirstChild;
        std::vector<uint64_t> parents;

        if (ImGui::TreeNode(scene.SceneName().c_str()))
        {
            CreateImGuiPopup(scene, rootGo);
            DragDropTargetAppend(scene.RootObject, scene);
            DragDropTargetBetween(entt::null, rootGo, parents, scene);

            while (currentDrawingEntity != entt::null)
            {
                currentDrawingEntity = RecursiveDraw(currentDrawingEntity, scene, parents);
                parents.clear();
            }
            ImGui::TreePop();
        }
        ImGui::End();

        ////
        // Inspector Panel
        ////
        ImGui::Begin("Inspector");
        static bool updatingName = false;
        static char editNameBuffer[128];
        if (entitySelected != -1)
        {
            goon::GameObject gameobject{(entt::entity)entitySelected, &scene};
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
                    // gameobject.GetComponent<goon::TagComponent>().set_tag(std::string(str0));
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
                auto uniqueInt = gameobject.GetComponentUniqueIntImGui<goon::HierarchyComponent>();
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
                auto uniqueInt = gameobject.GetComponentUniqueIntImGui<goon::IdComponent>();
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

        ImGui::Render();
        application.StartDrawFrame();
        editor.DrawImGuiFrame();
        application.EndDrawFrame();
        // TODO reenable sound, or put into a system.
        // UpdateSoundBro();
    }

    editor.ExitImGui();
    application.DestroyWindow();
    application.ExitSdl();

    return 0;
}

static entt::entity RecursiveDraw(entt::entity entity, goon::Scene &scene, std::vector<uint64_t> &parents)
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    goon::GameObject gameobject{entity, &scene};
    auto &hierarchyComponent = gameobject.GetComponent<goon::HierarchyComponent>();
    auto &tagComponent = gameobject.GetComponent<goon::TagComponent>();
    bool isInactive = gameobject.HasComponent<goon::InactiveComponent>();
    if (isInactive) // Try next child if we are inactive, this breaks out early.
        return hierarchyComponent.NextChild;
    if (hierarchyComponent.FirstChild == entt::null) // If we have no children, we draw a selectable.
    {
        ImGui::PushID(gameobject.GetGameobjectUniqueIntImgui());
        if (ImGui::Selectable(tagComponent, gameobject.GetID() == entitySelected))
        {
            entitySelected = gameobject.GetID();
        }
        DragDropSource(entity, tagComponent.Tag);
        DragDropTargetAppend(entity, scene);
        ImGui::PopID();
        CreateImGuiPopup(scene, entity);
        DragDropTargetBetween(entity, hierarchyComponent.Parent, parents, scene);
    }
    else // If we do have children, then Create a drag/drop target after the tree is created.
    {
        auto node_flags = base_flags;
        if (entitySelected == gameobject.GetID())
        {
            node_flags |= 1 << 0;
        }
        bool node_open = ImGui::TreeNodeEx(gameobject.GetGameobjectUniqueIntImgui(), node_flags, tagComponent.Tag.c_str());
        DragDropSource(entity, tagComponent.Tag);
        DragDropTargetAppend(entity, scene);
        // We keep this default(passing in null), so we know that this drop target will add it FIRST in the parents children.
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            entitySelected = gameobject.GetID();

        CreateImGuiPopup(scene, entity);
        if (node_open)
        {
            parents.push_back(gameobject.GetID());
            DragDropTargetBetween(entt::null, entity, parents, scene);
            auto nextChild = hierarchyComponent.FirstChild;
            while (nextChild != entt::null)
            {
                nextChild = RecursiveDraw(nextChild, scene, parents);
                std::remove(parents.begin(), parents.end(), gameobject.GetID());
            }
            ImGui::TreePop();
        }
        DragDropTargetBetween(entity, hierarchyComponent.Parent, parents, scene);
    }
    return hierarchyComponent.NextChild;
}

// TODO we need to change this from int to uint64_t
static void CreateImGuiPopup(goon::Scene &scene, entt::entity entityRightClicked)
{
    if (ImGui::BeginPopupContextItem(nullptr)) // entityTag is used as a id for the popup
    {
        entitySelected = (uint64_t)entityRightClicked;
        std::string name = "Unnamed";

        if (ImGui::Button("Create Gameobject at root"))
        {
            scene.CreateGameObject(name, scene.RootObject);
            ImGui::CloseCurrentPopup();
        }
        if (scene.RootObject != entityRightClicked)
        {
            if (ImGui::Button("Create child Gameobject"))
            {
                scene.CreateGameObject(name, entityRightClicked);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Delete GameObject"))
            {
                auto gameobject = goon::GameObject{entityRightClicked, &scene};
                auto hierarchy = gameobject.GetComponent<goon::HierarchyComponent>();
                auto parentGameobject = goon::GameObject{hierarchy.Parent, &scene};
                parentGameobject.RemoveChildEntity(entityRightClicked);
                scene.DestroyGameObject(entitySelected);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
}
template <typename T>
static bool RemoveComponentPopup(goon::Scene &scene, entt::entity entityRightClicked)
{
    bool removedComponent = false;
    if (ImGui::BeginPopupContextItem(nullptr)) // entityTag is used as a id for the popup
    {
        if (ImGui::Button("Remove Component"))
        {
            goon::GameObject go{entityRightClicked, &scene};
            go.RemoveComponent<T>();
            ImGui::CloseCurrentPopup();
            removedComponent = true;
        }
        ImGui::EndPopup();
    }
    return removedComponent;
}

static void DragDropSource(entt::entity entity, std::string &entityName)
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

// Should get rid of this function, and merge with the between, as that one checks the parents so is safer.
static void DragDropTargetAppend(entt::entity appendEntity, goon::Scene &scene)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("test"))
        {
            auto appendGO = goon::GameObject{appendEntity, &scene};
            IM_ASSERT(payload->DataSize == sizeof(uint64_t));
            uint64_t payload_n = *(const uint64_t *)payload->Data;
            goon::GameObject sourceGameObject{(entt::entity)payload_n, &scene};
            auto &hierarchy = sourceGameObject.GetComponent<goon::HierarchyComponent>();
            auto oldParentGameObject = goon::GameObject{hierarchy.Parent, &scene};
            oldParentGameObject.RemoveChildEntity((entt::entity)sourceGameObject);
            appendGO.AppendChildEntity((entt::entity)payload_n);
        }
        ImGui::EndDragDropTarget();
    }
}

// This should be called before first child, and after every child except the last child.
static void DragDropTargetBetween(entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents, goon::Scene &scene)
{
    static ImVec2 hoverSeparatorSize = {200, 5};
    if (thisFrameDrag || lastFrameDrag)
    {
        ImVec2 size = (thisFrameDrag || lastFrameDrag) ? hoverSeparatorSize : ImVec2(0, 0);
        ImGui::Dummy(size);
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("test"))
            {
                std::cout << "Parent is " << (uint64_t)parent << std::endl;
                IM_ASSERT(payload->DataSize == sizeof(uint64_t));
                uint64_t payload_n = *(const uint64_t *)payload->Data;
                // Gameobject that was dropped here
                goon::GameObject gameobject{(entt::entity)payload_n, &scene};
                auto &hierarchy = gameobject.GetComponent<goon::HierarchyComponent>();
                if (previousChild != gameobject)
                {
                    // Check if this gameobject is not inside of it's parents, this will prevent treenodes from being dropped into each other.
                    // is_tree and parents are needed for this, may be a better way?
                    if (std::find(parents.begin(), parents.end(), gameobject.GetID()) == parents.end())
                    {
                        auto oldParentGameObject = goon::GameObject{hierarchy.Parent, &scene};
                        oldParentGameObject.RemoveChildEntity((entt::entity)gameobject);
                        auto newParentGameObject = goon::GameObject{parent, &scene};
                        newParentGameObject.AddChildEntity((entt::entity)gameobject.GetID(), previousChild);
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
}
