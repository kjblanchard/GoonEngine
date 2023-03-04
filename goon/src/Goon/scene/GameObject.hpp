#pragma once
#include <entt/entity/registry.hpp>
#include <Goon/scene/Scene.hpp>

// entt::registry registry;
// const auto entity = registry.create();
// auto view = registry.view<const position, velocity>();

namespace goon
{

    class GameObject
    {
    public:
        GameObject(entt::entity entityId, Scene *scene);
        ~GameObject();
        template <typename T, typename... Args>
        void AddComponent(Args&&... args)
        {
            _scene->Registry().emplace<T>(_entityId, std::forward<Args>(args)...);
        }
        template <typename T>
        T& GetComponent()
        {
            return _scene->Registry().get<T>(_entityId);

        }

    private:
        entt::entity _entityId;
        Scene *_scene;
    };

}