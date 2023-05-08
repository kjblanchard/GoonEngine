#include <GoonPlatforms/Window/SdlWindowApi.hpp>
#include <glad/glad.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <Goon/core/Log.hpp>

#define GN_BACKEND_GL

namespace goon
{
        WindowApi::API WindowApi::s_Api = WindowApi::API::SDL2;

        SdlWindowApi::SdlWindowApi(const WindowProps &props)
        {
                if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
                {
                        GN_CORE_FATAL("Error initializing SDL, {}\n", SDL_GetError());
                }
                SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
                _window = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.Width, props.Height, window_flags);
        }
        SdlWindowApi::~SdlWindowApi()
        {
                SDL_DestroyWindow(_window);
                SDL_Quit();
        }

        void SdlWindowApi::OnUpdate()
        {
        }
        void *SdlWindowApi::GetNativeWindow() const
        {
                return _window;
        }

        WindowApi *WindowApi::Create(const WindowProps &props)
        {
                return new SdlWindowApi(props);
        }

}
