#include <gnpch.hpp>
#include <Goon/Renderer/RendererAPI.hpp>
#include <Goon/Renderer/Buffer.hpp>

// Backend Includes.
#include <GoonPlatforms/Rendering/OpenGL/OpenGLBuffer.hpp>

namespace Goon
{
    VertexBuffer *VertexBuffer::Create(float *vertices, uint32_t size)
    {
        auto rendererType = RendererAPI::GetAPI();
        if (rendererType == RendererAPI::API::OpenGL)
        {
            return new OpenGLVertexBuffer(vertices, size);
        }
        else
        {
            GN_CORE_FATAL("Backend type not implemented");
        }
        return nullptr;
    }

    IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t size)
    {
        auto rendererType = RendererAPI::GetAPI();
        if (rendererType == RendererAPI::API::OpenGL)
        {
            return new OpenGLIndexBuffer(indices, size);
        }
        else
        {
            GN_CORE_FATAL("Backend type not impmented");
        }
        return nullptr;
    }

}