#pragma once
#include <Goon/Renderer/RendererAPI.hpp>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <Goon/core/Log.hpp>

#ifdef GN_BACKEND_SDL
#include <SDL_video.h>
#endif

namespace goon
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        OpenGLRendererAPI(WindowApi& window);
        void SetClearColor(const glm::vec4 &color) override;
        void Clear() override;
        void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override;
        void SwapBuffers() override;

        static RendererAPI *Create(WindowApi &window);

    private:
        static SDL_GLContext *context;
    };
}