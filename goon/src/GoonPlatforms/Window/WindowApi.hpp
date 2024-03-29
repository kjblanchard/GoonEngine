#pragma once
/**
 * @file WindowApi.hpp
 * @brief Defines what a window should be
 * @author Kevin Blanchard
 * @version 0.1.0
 * @date 2023-5-7
 */
#include <gnpch.hpp>

namespace goon
{
    enum class WindowTypes
    {
        Default,
        SDL2,
    };

    /**
     * @brief The Properties of a window needed to create one
     */
    struct WindowProps
    {
        /**
         * @brief Name of the window
         */
        std::string Title;
        /**
         * @brief WIdth of a window
         */
        unsigned int Width;
        /**
         * @brief Height of a window
         */
        unsigned int Height;
        WindowProps(const std::string &title = "Goon Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height) {}
    };

    /**
     * @brief Interface representing a Desktop window
     */
    class WindowApi
    {
    public:
        virtual ~WindowApi() {}
        /**
         * @brief Update function for the window
         */
        virtual void OnUpdate() = 0;
        /**
         * @brief Width of the window
         *
         * @return Width of the window
         */
        virtual unsigned int GetWidth() const = 0;
        /**
         * @brief Height of the window
         *
         * @return The height of the window
         */
        virtual unsigned int GetHeight() const = 0;
        /**
         * @brief Sets the window callback function
         *
         * @param callback The callback function to bind
         */
        virtual std::pair<unsigned int, unsigned int> GetWindowSize() = 0;

        /**
         * @brief Sets the windows vsync
         *
         * @param enabled Should vsync be enabled
         */
        virtual void SetVSync(bool enabled) = 0;
        /**
         * @brief Is Vsync enabled on the window
         *
         * @return True if vsync is enabled.
         */
        virtual bool IsVSync() const = 0;
        /**
         * @brief The DPI scaling of the current window, this is needed as glfw / mac does dpi scaling funky.
         *
         * @return The current scaling x/y in the window
         */
        virtual std::pair<float, float> GetWindowContentScaling() const = 0;
        /**
         * @brief Gets the window size in pixels.  This is needed as mac is funky
         *
         * @return The amount of pixels, x and y in a pair
         */
        virtual std::pair<int, int> GetWindowFrameBufferSize() const = 0;
        /**
         * @brief Creates a window.
         *
         * @param props THe window props to create the window with
         *
         * @return The Created window.
         */
        static WindowApi *Create(const WindowProps &props = WindowProps());
    };
}