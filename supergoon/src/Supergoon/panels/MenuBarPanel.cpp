#include <Supergoon/panels/MenuBarPanel.hpp>
#include <Goon/scene/Scene.hpp>
#include <imgui.h>
namespace goon
{
    MenuBarPanel::~MenuBarPanel()
    {
    }
    void MenuBarPanel::DrawPanel(Scene *scene)
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "ctrl + s"))
                {
                    scene->SerializeScene();
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
}