#pragma once

namespace goon
{
    class Application
    {
    public:
        Application();
        void Init();

        static Application *GetApplication() { return _application; }

    private:
        static Application *_application;
    };
}