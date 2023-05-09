#include <Goon/Renderer/Texture.hpp>
#include <Goon/Renderer/RendererAPI.hpp>
#include <GoonPlatforms/Rendering/OpenGL/OpenGLTexture.hpp>
namespace goon
{
    Texture *Texture::Create(std::string &filename)
    {
        auto rendererType = RendererAPI::GetAPI();
        if (rendererType == RendererAPI::API::OpenGL)
        {
            return new OpenGLTexture(filename);
        }
        else
        {
            GN_CORE_FATAL("Backend type not implemented");
        }
        return nullptr;
    }

}