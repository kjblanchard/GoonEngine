#pragma once
//No be here
#include <SDL_video.h>
#include <Goon/Renderer/VertexArray.hpp>

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
        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
        //TODO these need to move
        static SDL_GLContext GetGLContext();
        static SDL_Window* GetWindow();
    };
}