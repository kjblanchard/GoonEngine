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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void OpenGLTexture::UnBind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void OpenGLTexture::Load()
    {
        auto asset = ImageAsset(_filename);
        asset.Load();
        if (!asset.GetLoadedData())
            return;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, asset.GetWidth(), asset.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, asset.GetLoadedData());

        glGenerateMipmap(GL_TEXTURE_2D);
    }
}