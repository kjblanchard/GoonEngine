#pragma once
#include <entt/entity/registry.hpp>

namespace goon
{
    class GameObject;
    class Scene
    {
    public:
        Scene();
        GameObject CreateGameObject(std::string &name);

        entt::registry& Registry() { return _registry; }
        template<typename T>
		void OnComponentAdded(GameObject entity, T& component);

    private:
        entt::registry _registry;
    };
}