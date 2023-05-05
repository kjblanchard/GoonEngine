#include <Goon/core/Application.hpp>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

namespace goon
{
    Application *Application::_application = nullptr;
    Application::Application()
    {
        if (!_application)
            _application = this;
    }
    bool Application::InitializeSDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            return -1;
        }

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char *glsl_version = "#version 100";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
        // GL 3.2 Core + GLSL 150
        const char *glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
        // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

        // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        _window = SDL_CreateWindow("Supergoon Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
        SDL_GLContext gl_context = SDL_GL_CreateContext(_window);
        SDL_GL_MakeCurrent(_window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync

        if (_context == NULL)
        {
            SDL_Log("Error creating GL context!!");
            return 0;
        }
        SDL_RendererInfo info;
        return 0;
    }
    void Application::ResizeWindow()
    {
#ifdef GN_PLATFORM_MACOS
        int width, height, rwidth, rheight;
        float scalex, scaley;
        SDL_GetWindowSize(_window, &width, &height);

        SDL_GL_GetDrawableSize(_window, &rwidth, &rheight);


        // SDL_GetRendererOutputSize(_renderer, &rwidth, &rheight);
        scalex = rwidth / width;
        scaley = rheight / height;
        glViewport(0, 0, width * scalex, height* scaley);
        // SDL_RenderSetScale(_renderer, scalex, scaley);
#endif
    }

    void Application::StartDrawFrame()
    {

        // SDL_SetRenderDrawColor(_renderer, (Uint8)(255), (Uint8)(255), (Uint8)(255), (Uint8)(255));
        // SDL_RenderClear(_renderer);


        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void Application::EndDrawFrame()
    {
        // SDL_RenderPresent(_renderer);
        SDL_GL_SwapWindow(_window);
    }
    void Application::DestroyWindow()
    {
        // SDL_DestroyRenderer(_renderer);
        // SDL_DestroyWindow(_window);
    }
    void Application::ExitSdl()
    {
        SDL_Quit();
    }

}