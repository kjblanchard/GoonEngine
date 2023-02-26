#pragma once
#include <entt/entity/registry.hpp>
#include <Goon/scene/GameObject.hpp>

namespace goon
{
    class Scene
    {
    public:
        Scene();
        GameObject CreateGameObject(std::string &name);

        entt::registry& Registry() { return _registry; }

    private:
        entt::registry _registry;
    };
}