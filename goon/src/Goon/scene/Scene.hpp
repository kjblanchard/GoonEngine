#pragma once
#include <entt/entity/registry.hpp>
#include <string>

namespace YAML
{
    class Node;
}
namespace goon
{
    enum class GameObjectComponents
    {
        None = 0,
        BGM = 1,
    };
    class GameObject;
    class Scene
    {
    public:
        static Scene *GetScene() { return _scene; }
        Scene();
        GameObject CreateGameObject(std::string &name, entt::entity parent = entt::null);
        void DestroyGameObject(uint64_t entityId);
        GameObject GetGameObjectById(uint64_t entityId);
        entt::registry &Registry() { return _registry; }
        template <typename T>
        void OnComponentAdded(GameObject entity, T &component);
        void SerializeScene();
        void DeSerializeScene();
        std::string SceneName() { return _sceneName; }
        entt::entity RootObject;

    private:
        std::string _sceneName = "default";
        static Scene *_scene;
        entt::registry _registry;
        entt::entity CreateGameObjectFromYaml(uint64_t entityId, entt::entity parent, YAML::Node &gameObjectNode);
    };
}