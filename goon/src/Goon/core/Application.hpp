#pragma once

namespace goon
{
    class Application
    {
    public:
    Application();
        bool InitializeSDL();

        // Window stuff
        /**
         * @brief Handle macos on highdpi, this is a short term shim that should be removed eventually, or put in renrerer.
         *
         */
        void ResizeWindow();
        void StartDrawFrame();
        void EndDrawFrame();
        void DestroyWindow();
        void ExitSdl();
        static Application* GetApplication() { return _application; }

    private:
        static Application* _application;

    };
}