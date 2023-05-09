#pragma once
#include <Goon/Renderer/Texture.hpp>
#include <glad/glad.h>

namespace goon
{

    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(std::string &filename);
        void Bind() override;
        void UnBind() override;
        void Load() override;
    };
}