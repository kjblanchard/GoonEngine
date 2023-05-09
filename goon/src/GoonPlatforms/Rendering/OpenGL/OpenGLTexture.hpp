#pragma once
#include <Goon/Renderer/Texture.hpp>

namespace goon
{
    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(std::string &filename);
        virtual void Bind() override;
        virtual void Load() override;
    };
}