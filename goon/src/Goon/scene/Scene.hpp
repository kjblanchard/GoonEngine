#pragma once
#include <entt/entity/registry.hpp>

namespace YAML
{
    class Node;
}
namespace goon
{
    class GameObject;
    class Scene
    {
    public:
        Scene();
        GameObject CreateGameObject(std::string &name, entt::entity parent = entt::null);

        entt::registry &Registry() { return _registry; }
        template <typename T>
        void OnComponentAdded(GameObject entity, T &component);
        void SerializeScene();
        void DeSerializeScene();
        std::string SceneName() { return _sceneName; }

        entt::entity RootObject;

    private:
        std::string _sceneName = "default";
        entt::registry _registry;
        entt::entity CreateGameObjectFromYaml(uint64_t entityId, entt::entity parent, YAML::Node &gameObjectNode);
    };
}