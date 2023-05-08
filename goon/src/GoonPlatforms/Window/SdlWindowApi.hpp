#pragma once
#include <Goon/Window/WindowApi.hpp>
#include <SDL.h>

namespace goon
{
    class SdlWindowApi : public WindowApi
    {
    public:
        SdlWindowApi(const WindowProps& props);
        ~SdlWindowApi() override;
        /**
         * @brief Update function for the window
         */
        void OnUpdate() override;
        /**
         * @brief Width of the window
         *
         * @return Width of the window
         */
        unsigned int GetWidth() const override {}
        /**
         * @brief Height of the window
         *
         * @return The height of the window
         */
        unsigned int GetHeight() const override {}
        std::pair<unsigned int, unsigned int> GetWindowSize() override {}

        /**
         * @brief Sets the windows vsync
         *
         * @param enabled Should vsync be enabled
         */
        void SetVSync(bool enabled) override {}
        /**
         * @brief Is Vsync enabled on the window
         *
         * @return True if vsync is enabled.
         */
        bool IsVSync() const override {}
        /**
         * @brief The DPI scaling of the current window, this is needed as glfw / mac does dpi scaling funky.
         *
         * @return The current scaling x/y in the window
         */
        std::pair<float, float> GetWindowContentScaling() const override {}
        /**
         * @brief Gets the window size in pixels.  This is needed as mac is funky
         *
         * @return The amount of pixels, x and y in a pair
         */
        std::pair<int, int> GetWindowFrameBufferSize() const override {}
        /**
         * @brief Creates a window.
         *
         * @param props THe window props to create the window with
         *
         * @return The Created window.
         */
        void* GetNativeWindow() const override;

    private:
        SDL_Window *_window;
    };
}