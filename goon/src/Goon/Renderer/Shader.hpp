#pragma once

namespace goon
{
    class Shader
    {

    public:
        virtual ~Shader() {}
        virtual void Bind() = 0;
        static Shader *Create(const std::string &shaderSourcePath);
        virtual void BindTexture() = 0;
    };
}