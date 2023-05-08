#include <gnpch.hpp>
#include <Goon/Renderer/RenderCommand.hpp>

// Backend includes.
#include <GoonPlatforms/Rendering/OpenGL/OpenGLRendererAPI.hpp>

namespace Goon
{
        RendererAPI *RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}