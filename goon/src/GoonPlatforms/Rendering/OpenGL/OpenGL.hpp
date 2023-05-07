#pragma once
//No be here
#include <SDL_video.h>

namespace goon
{
    class WindowApi;
    class OpenGL
    {
    public:
        static void Init();
        static void ResizeWindow();
        static void StartDrawFrame();
        static void EndDrawFrame();
        //TODO these need to move
        static SDL_GLContext GetGLContext();
        static SDL_Window* GetWindow();
    };
}