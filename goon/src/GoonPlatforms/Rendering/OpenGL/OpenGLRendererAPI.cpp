#include <gnpch.hpp>
#include <glad/glad.h>
#include <GoonPlatforms/Rendering/OpenGL/OpenGLRendererAPI.hpp>

#include <Goon/Window/WindowApi.hpp>

namespace goon
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
    RendererAPI *RendererAPI::s_RendererApi = nullptr;
    SDL_GLContext *OpenGLRendererAPI::context = nullptr;

    OpenGLRendererAPI::OpenGLRendererAPI(WindowApi &window)
    {
#ifdef GN_BACKEND_SDL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        m_Window = &window;

        auto nativeWindow = (SDL_Window *)m_Window->GetNativeWindow();
        SDL_GLContext gl_context = SDL_GL_CreateContext(nativeWindow);
        SDL_GL_MakeCurrent(nativeWindow, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync
        int status = gladLoadGLLoader(SDL_GL_GetProcAddress);
        if (!status)
        {
            GN_CORE_FATAL("Could not load opengl, exiting. . . ");
        }
        context = &gl_context;
        auto vendor = (char *)glGetString(GL_VENDOR);
        auto renderer = (char *)glGetString(GL_RENDERER);
        auto version = (char *)glGetString(GL_VERSION);
        GN_CORE_INFO("Vendor {0}/ Renderer: {1}, version {2}", vendor, renderer, version);
#endif
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }
    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
    {
        glDrawElements(GL_TRIANGLES,
                       vertexArray->GetIndexBuffer()->GetCount(),
                       GL_UNSIGNED_INT,
                       nullptr);
    }

    RendererAPI *RendererAPI::Create(WindowApi &window)
    {
        //This is jank
        RendererAPI::s_RendererApi = new OpenGLRendererAPI(window);
        return RendererAPI::s_RendererApi;
    }

    void OpenGLRendererAPI::SwapBuffers()
    {
#ifdef GN_BACKEND_SDL
        SDL_GL_SwapWindow((SDL_Window *)m_Window);
#endif
    }

}