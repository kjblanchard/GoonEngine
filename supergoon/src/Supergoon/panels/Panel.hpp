#pragma once

namespace goon
{
    class Scene;
    class Panel
    {
    public:
        virtual void DrawPanel(Scene *scene) = 0;
        virtual ~Panel() = default;
    };
}