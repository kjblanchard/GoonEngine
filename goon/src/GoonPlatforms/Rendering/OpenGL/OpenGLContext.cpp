#include <gnpch.hpp>
#include <glad/glad.h>
#include <Goon/core/Log.hpp>
#include <GoonPlatforms/Rendering/OpenGL/OpenGLContext.hpp>

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

namespace Goon
{

    OpenGLContext::OpenGLContext(SDL_Window *window)
        : GraphicsContext(), m_window(window)
    {
    }

    void OpenGLContext::Init()
    {
        int status = gladLoadGLLoader(SDL_GL_GetProcAddress);

        // TODO check the status is good.
        auto vendor = (char *)glGetString(GL_VENDOR);
        auto renderer = (char *)glGetString(GL_RENDERER);
        auto version = (char *)glGetString(GL_VERSION);
        GN_CORE_INFO("Vendor {0}/ Renderer: {1}, version {2}", vendor, renderer, version);
#ifdef GN_WINDOW_SDL

        SDL_GLContext gl_context = SDL_GL_CreateContext(m_window);
        SDL_GL_MakeCurrent(m_window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync
        if (!status)
        {
            GN_CORE_FATAL("Could not load opengl, exiting. . . ");
        }
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_window);
    }
}