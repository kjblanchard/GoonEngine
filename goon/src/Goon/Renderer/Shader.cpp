#include <gnpch.hpp>
#include <Goon/Renderer/Shader.hpp>
#include <Goon/Renderer/RendererAPI.hpp>
#include <GoonPlatforms/Rendering/OpenGL/OpenGLShader.hpp>
#include <Goon/core/Log.hpp>
#include <GoonTools/io.hpp>

namespace goon
{

    Shader *Shader::Create(const std::string &shaderSourcePath)
    {
        auto rendererType = RendererAPI::GetAPI();
        if (rendererType == RendererAPI::API::OpenGL)
        {
            std::string vFilePath = "shaders/vertex/" + shaderSourcePath;
            std::string fFilePath = "shaders/fragment/" + shaderSourcePath;
            auto vshader = ReadFromFile(vFilePath);
            auto fshader = ReadFromFile(fFilePath);
            return new OpenGLShader(vshader, fshader);
        }
        else
        {
            GN_CORE_FATAL("The backend type is not currently implemented.");
            return nullptr;
        }
    }


}