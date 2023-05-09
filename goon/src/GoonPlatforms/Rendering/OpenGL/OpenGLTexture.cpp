#include <glad/glad.h>
#include <Goon/core/Log.hpp>
#include <GoonPlatforms/Rendering/OpenGL/OpenGLTexture.hpp>
#include <Goon/core/image_asset.hpp>

namespace goon
{

    OpenGLTexture::OpenGLTexture(std::string &filename)
        : Texture(filename)
    {
    }
    void OpenGLTexture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }
    void OpenGLTexture::Load()
    {
        auto asset = ImageAsset(_filename);
        asset.Load();
        if (!asset.GetLoadedData())
            return;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}