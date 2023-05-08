#include "gnpch.hpp"
#include <Goon/Renderer/VertexArray.hpp>
#include <Goon/Renderer/RendererAPI.hpp>
#include <GoonPlatforms/Rendering/OpenGL/OpenGLVertexArray.hpp>

namespace goon
{

    VertexArray* VertexArray::Create()
    {
        auto rendererType = RendererAPI::GetAPI();
        if (rendererType == RendererAPI::API::OpenGL)
        {
            return new OpenGLVertexArray();
        }
        else
        {
            GN_CORE_FATAL("The backend type is not currently implemented.");
        }
        return nullptr;

    }
}