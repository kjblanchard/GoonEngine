#pragma once
#include <SDL_events.h>
#include <entt/fwd.hpp>
#include <Supergoon/panels/MenuBarPanel.hpp>
#include <Supergoon/panels/HierarchyPanel.hpp>
#include <Supergoon/panels/InspectorPanel.hpp>
#include <Goon/scene/GameObject.hpp>

namespace goon
{
    class Scene;
    class EditorLayer
    {
    public:
        /**
         * @brief Initializes ImGui for use.
         */
        void InitializeImGui();
        /**
         * @brief Process events from SDL2
         */
        void ProcessImGuiEvents(SDL_Event *);
        /**
         * @brief Starts a new IMGUI frame for rendering
         */
        void ImGuiNewFrame();
        /**
         * @brief Draw all of the Imgui windows for the current scene
         */
        void ProcessImGuiFrame();
        /**
         * @brief Actually send imgui frame info to opengl
         */
        void DrawImGuiFrame();
        /**
         * @brief Close imgui
         */
        void ExitImGui();
        /**
         * @brief Loads the scene into the editor.  So that we know what to draw in the inspector, hierarchy, etc.
         * @param scene
         */
        void LoadScene(Scene &scene) { _scene = &scene; }
        /**
         * @brief Current entity selected in the scene.
         */
        entt::entity entitySelected;
        /**
         * @brief Used for generating a unique int from this gameobject
         *
         * @return void*
         */
        void *GetGameobjectUniqueIntImgui(GameObject *gameobject);
        /**
         * @brief Used for generating a unique int from the type sent in.  Probably should combine the two or remove.  This is used in editor for imgui, likely move it there..
         *
         * @tparam T
         * @return void*
         */
        template <typename T>
        void *GetComponentUniqueIntImGui(GameObject *gameobject);

    private:
        /**
         * @brief Creates all the imgui panels for the editor
         */
        void CreateAllPanels();
        /**
         * @brief Actually draw the imgui demo window if it is enabled.
         *
         */
        void DrawDebugWindow();

    private:
        std::unique_ptr<MenuBarPanel> _menuBarPanel;
        std::unique_ptr<HierarchyPanel> _hierarchyPanel;
        std::unique_ptr<InspectorPanel> _inspectorPanel;
        Scene *_scene;
    };

    template <typename T>
    void *EditorLayer::GetComponentUniqueIntImGui(GameObject *gameobject)
    {
        return (void *)(intptr_t)(gameobject->GetID() + typeid(T).hash_code());
    }

}
