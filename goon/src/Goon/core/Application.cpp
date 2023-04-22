#include <Goon/core/Application.hpp>

namespace goon
{
    bool Application::InitializeSDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            return -1;
        }
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        _window = SDL_CreateWindow("Supergoon Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        if (_renderer == NULL)
        {
            SDL_Log("Error creating SDL_Renderer!");
            return 0;
        }
        SDL_RendererInfo info;
        SDL_GetRendererInfo(_renderer, &info);
        SDL_Log("Current SDL_Renderer: %s", info.name);
        return 0;
    }
    void Application::ResizeWindow()
    {
#ifdef GN_PLATFORM_MACOS
        int width, height, rwidth, rheight;
        float scalex, scaley;
        SDL_GetWindowSize(_window, &width, &height);
        SDL_GetRendererOutputSize(_renderer, &rwidth, &rheight);
        scalex = rwidth / width;
        scaley = rheight / height;
        SDL_RenderSetScale(_renderer, scalex, scaley);
#endif
    }

    void Application::StartDrawFrame()
    {
        SDL_SetRenderDrawColor(_renderer, (Uint8)(255), (Uint8)(255), (Uint8)(255), (Uint8)(255));
        SDL_RenderClear(_renderer);
    }
    void Application::EndDrawFrame()
    {
        SDL_RenderPresent(_renderer);
    }
    void Application::DestroyWindow()
    {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }
    void Application::ExitSdl()
    {
        SDL_Quit();
    }

}