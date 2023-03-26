#pragma once
#include <entt/entity/registry.hpp>

namespace goon
{
    class GameObject;
    class Scene
    {
    public:
        Scene();
        GameObject CreateGameObject(std::string &name, entt::entity parent = entt::null);

        entt::registry& Registry() { return _registry; }
        template<typename T>
		void OnComponentAdded(GameObject entity, T& component);

        void SerializeScene();
        void DeSerializeScene();

        entt::entity RootObject;

    private:
        entt::registry _registry;
    };
}