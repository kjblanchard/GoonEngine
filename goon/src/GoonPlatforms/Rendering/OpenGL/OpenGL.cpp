#include <GoonPlatforms/Rendering/OpenGL/OpenGL.hpp>
#include <glad/glad.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <Goon/core/Log.hpp>

#define GN_WINDOW_SDL
#ifdef GN_WINDOW_SDL
#include <SDL_video.h>
#endif

namespace goon
{
    void OpenGL::StartDrawFrame()
    {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void OpenGL::EndDrawFrame()
    {
        SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
    }
    void OpenGL::ResizeWindow()
    {
        int width, height;
        SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
        glViewport(0, 0, width, height);
    }

    SDL_GLContext OpenGL::GetGLContext()
    {
        return SDL_GL_GetCurrentContext();
    }
    SDL_Window *OpenGL::GetWindow()
    {
        return SDL_GL_GetCurrentWindow();
    }

}