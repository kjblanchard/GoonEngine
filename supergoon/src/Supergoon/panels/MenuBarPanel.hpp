#pragma once
#include <Supergoon/panels/Panel.hpp>

namespace goon
{
    class MenuBarPanel : public Panel
    {
    public:
        MenuBarPanel() = default;
        ~MenuBarPanel();
        virtual void DrawPanel(Scene *scene);
    };
}