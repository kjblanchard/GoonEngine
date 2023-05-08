#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <Goon/Renderer/VertexArray.hpp>
#include <Goon/Window/WindowApi.hpp>

namespace goon
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1
        };

    public:
        virtual ~RendererAPI() = default;
        virtual void SetClearColor(const glm::vec4 &color) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) = 0;

        virtual void SwapBuffers() = 0;

        inline static API GetAPI() { return s_API; }
        inline static RendererAPI* GetRendererAPI() { return s_RendererApi; }

        static RendererAPI *Create(WindowApi& window);
        WindowApi* m_Window;

    protected:
        static API s_API;
        static RendererAPI* s_RendererApi;
    };
}
