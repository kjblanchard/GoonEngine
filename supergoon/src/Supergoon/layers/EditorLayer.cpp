#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <Goon/scene/Scene.hpp>
#include <entt/entity/entity.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Goon/scene/Scene.hpp>
#include <Goon/core/Application.hpp>
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
#include <Supergoon/layers/EditorLayer.hpp>

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
        DrawMainMenu();
        DrawDebugWindow();
        DrawHierarchy();
        DrawInspector();
        ImGui::Render();
    }

    void EditorLayer::DrawMainMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "ctrl + s"))
                {
                    _scene->SerializeScene();
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
    }
    void EditorLayer::DrawDebugWindow()
    {
        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);
    }
    void EditorLayer::DrawInspector()
    {
        ////
        // Inspector Panel
        ////
        ImGui::Begin("Inspector");
        static bool updatingName = false;
        static char editNameBuffer[128];
        if (entitySelected != entt::null)
        {
            goon::GameObject gameobject{(entt::entity)entitySelected, _scene};
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
                    if (!RemoveComponentPopup<goon::BgmComponent>((entt::entity)gameobject.GetID()))
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
    }
    void EditorLayer::DrawHierarchy()
    {
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
        goon::GameObject rootGo{_scene->RootObject, _scene};
        auto &rootHierarchy = rootGo.GetComponent<goon::HierarchyComponent>();
        entt::entity currentDrawingEntity = rootHierarchy.FirstChild;
        std::vector<uint64_t> parents;

        if (ImGui::TreeNode(_scene->SceneName().c_str()))
        {
            CreateImGuiPopup(rootGo);
            DragDropTargetAppend(_scene->RootObject, parents);
            DragDropTargetBetween(entt::null, rootGo, parents);

            while (currentDrawingEntity != entt::null)
            {
                currentDrawingEntity = RecursiveDraw(currentDrawingEntity, parents);
                parents.clear();
            }
            ImGui::TreePop();
        }
        ImGui::End();
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

    entt::entity EditorLayer::RecursiveDraw(entt::entity entity, std::vector<uint64_t> &parents)
    {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        goon::GameObject gameobject{entity, _scene};
        auto &hierarchyComponent = gameobject.GetComponent<goon::HierarchyComponent>();
        auto &tagComponent = gameobject.GetComponent<goon::TagComponent>();
        bool isInactive = gameobject.HasComponent<goon::InactiveComponent>();
        if (isInactive) // Try next child if we are inactive, this breaks out early.
            return hierarchyComponent.NextChild;
        if (hierarchyComponent.FirstChild == entt::null) // If we have no children, we draw a selectable.
        {
            ImGui::PushID(gameobject.GetGameobjectUniqueIntImgui());
            if (ImGui::Selectable(tagComponent, gameobject.GetEntity() == entitySelected))
            {
                entitySelected = gameobject.GetEntity();
            }
            DragDropSource(entity, tagComponent.Tag);
            DragDropTargetAppend(entity, parents);
            ImGui::PopID();
            CreateImGuiPopup(entity);
            DragDropTargetBetween(entity, hierarchyComponent.Parent, parents);
        }
        else // If we do have children, then Create a drag/drop target after the tree is created.
        {
            auto node_flags = base_flags;
            if (entitySelected == gameobject.GetEntity())
            {
                node_flags |= 1 << 0;
            }
            bool node_open = ImGui::TreeNodeEx(gameobject.GetGameobjectUniqueIntImgui(), node_flags, tagComponent.Tag.c_str());
            DragDropSource(entity, tagComponent.Tag);
            DragDropTargetAppend(entity, parents);
            // We keep this default(passing in null), so we know that this drop target will add it FIRST in the parents children.
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                entitySelected = gameobject.GetEntity();

            CreateImGuiPopup(entity);
            if (node_open)
            {
                auto newParents = std::vector<uint64_t>(parents);
                newParents.push_back(gameobject.GetID());
                DragDropTargetBetween(entt::null, entity, newParents);
                auto nextChild = hierarchyComponent.FirstChild;
                while (nextChild != entt::null)
                {
                    nextChild = RecursiveDraw(nextChild, newParents);
                    std::remove(newParents.begin(), newParents.end(), gameobject.GetID());
                }
                ImGui::TreePop();
            }
            DragDropTargetBetween(entity, hierarchyComponent.Parent, parents);
        }
        return hierarchyComponent.NextChild;
    }

    void EditorLayer::CreateImGuiPopup(entt::entity entityRightClicked)
    {
        if (ImGui::BeginPopupContextItem(nullptr)) // entityTag is used as a id for the popup
        {
            entitySelected = entityRightClicked;
            std::string name = "Unnamed";

            if (ImGui::Button("Create Gameobject at root"))
            {
                _scene->CreateGameObject(name, _scene->RootObject);
                ImGui::CloseCurrentPopup();
            }
            if (_scene->RootObject != entityRightClicked)
            {
                if (ImGui::Button("Create child Gameobject"))
                {
                    _scene->CreateGameObject(name, entityRightClicked);
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Delete GameObject"))
                {
                    auto gameobject = goon::GameObject{entityRightClicked, _scene};
                    auto hierarchy = gameobject.GetComponent<goon::HierarchyComponent>();
                    auto parentGameobject = goon::GameObject{hierarchy.Parent, _scene};
                    parentGameobject.RemoveChildEntity(entityRightClicked);
                    _scene->DestroyGameObject((uint64_t)entitySelected);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }
    template <typename T>
    bool EditorLayer::RemoveComponentPopup(entt::entity entityRightClicked)
    {
        bool removedComponent = false;
        if (ImGui::BeginPopupContextItem(nullptr)) // entityTag is used as a id for the popup
        {
            if (ImGui::Button("Remove Component"))
            {
                goon::GameObject go{entityRightClicked, _scene};
                go.RemoveComponent<T>();
                ImGui::CloseCurrentPopup();
                removedComponent = true;
            }
            ImGui::EndPopup();
        }
        return removedComponent;
    }

    void EditorLayer::DragDropSource(entt::entity entity, std::string &entityName)
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

    void EditorLayer::DragDropTargetAppend(entt::entity appendEntity, std::vector<uint64_t> &parents)
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("test"))
            {
                auto appendGO = goon::GameObject{appendEntity, _scene};
                IM_ASSERT(payload->DataSize == sizeof(uint64_t));
                uint64_t droppedEntityId = *(const uint64_t *)payload->Data;
                goon::GameObject droppedGameObject{(entt::entity)droppedEntityId, _scene};
                if (std::find(parents.begin(), parents.end(), droppedGameObject.GetID()) == parents.end())
                {
                    auto &hierarchy = droppedGameObject.GetComponent<goon::HierarchyComponent>();
                    auto oldParentGameObject = goon::GameObject{hierarchy.Parent, _scene};
                    oldParentGameObject.RemoveChildEntity((entt::entity)droppedGameObject);
                    appendGO.AppendChildEntity((entt::entity)droppedEntityId);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    void EditorLayer::DragDropTargetBetween(entt::entity previousChild, entt::entity parent, std::vector<uint64_t> &parents)
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
                goon::GameObject droppedGameObject{(entt::entity)droppedEntityId, _scene};
                if (previousChild != droppedGameObject)
                {
                    if (std::find(parents.begin(), parents.end(), droppedGameObject.GetID()) == parents.end())
                    {
                        auto &hierarchy = droppedGameObject.GetComponent<goon::HierarchyComponent>();
                        auto oldParentGameObject = goon::GameObject{hierarchy.Parent, _scene};
                        oldParentGameObject.RemoveChildEntity((entt::entity)droppedGameObject);
                        auto newParentGameObject = goon::GameObject{parent, _scene};
                        newParentGameObject.AddChildEntity((entt::entity)droppedGameObject.GetID(), previousChild);
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

}
