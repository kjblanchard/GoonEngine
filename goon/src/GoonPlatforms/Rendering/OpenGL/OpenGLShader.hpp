#pragma once
#include <gnpch.hpp>
#include <Goon/Renderer/Shader.hpp>

namespace goon
{
    class OpenGLShader: public Shader
    {
    public:
        OpenGLShader(const std::string& vertex_shader, const std::string& fragment_shader);
        void Bind() override;
        void Unbind();
        void BindTexture() override;
        std::string m_ShaderData;
    private:
        std::string LoadDataFromFile(std::string filepath);
        unsigned int m_Id;
    };
}