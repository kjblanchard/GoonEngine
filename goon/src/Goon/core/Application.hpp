#pragma once
#include <SDL.h>

namespace goon
{
    class Application
    {
    public:
    Application();
        bool InitializeSDL();
        // SDL_Renderer *GetRenderer() { return _renderer; }
        SDL_GLContext* *GetGLContext() { return _context; }
        SDL_Window *GetWindow() { return _window; }

        // Window stuff
        /**
         * @brief Handle macos on highdpi, this is a short term shim that should be removed eventually, or put in renrerer.
         *
         */
        void ResizeWindow();
        void StartDrawFrame();
        void EndDrawFrame();
        void DestroyWindow();
        void ExitSdl();
        static Application* GetApplication() { return _application; }

    private:
        SDL_Window *_window;
        // SDL_Renderer *_renderer;
        SDL_GLContext *_context;
        static Application* _application;

    };
}