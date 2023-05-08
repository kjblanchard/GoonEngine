#include <Goon/core/Application.hpp>
#include <Goon/core/Log.hpp>

#ifdef GN_BACKEND_OPENGL
#include <GoonPlatforms/Rendering/OpenGL/OpenGLRendererAPI.hpp>
#endif

#ifdef GN_BACKEND_SDL
#include <GoonPlatforms/Window/SdlWindowApi.hpp>
#endif

namespace goon
{
    Application *Application::_application = nullptr;
    Application::Application()
    {
        auto window = WindowApi::Create();
        auto renderer = RendererAPI::Create(*window);
        if (!_application)
            _application = this;
    }

    void Init()
    {
    }

}