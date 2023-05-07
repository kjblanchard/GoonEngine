#include <Goon/core/Application.hpp>
#include <Goon/core/Log.hpp>

namespace goon
{
    Application *Application::_application = nullptr;
    Application::Application()
    {
        if (!_application)
            _application = this;
    }
    void Application::ResizeWindow()
    {
    }

}