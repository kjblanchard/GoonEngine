#pragma once
#include <SDL_events.h>

namespace goon
{
    class EditorLayer
    {
    public:
        void InitializeImGui();
        void ProcessImGuiEvents(SDL_Event *);
        void ImGuiNewFrame();
        void DrawImGuiFrame();
        void ExitImGui();

        bool showDemoWindow = true;
    };
}