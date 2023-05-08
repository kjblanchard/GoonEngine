#pragma once
#include <Goon/Renderer/GraphicsContext.hpp>

struct SDL_Window;

namespace Goon {
    class OpenGLContext : public GraphicsContext
    {
        public:
            OpenGLContext(SDL_Window *window);
            virtual void Init() override;
            virtual void SwapBuffers() override;

        private:
            SDL_Window *m_window;
    };
}