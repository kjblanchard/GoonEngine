#pragma once
#include <entt/entity/registry.hpp>

// entt::registry registry;
// const auto entity = registry.create();
// auto view = registry.view<const position, velocity>();

namespace goon
{
    class Scene;

    class GameObject
    {
    public:
        GameObject(entt::entity entityId, Scene *scene);
        ~GameObject();

    private:
        entt::entity _entityId;
        Scene *_scene;
    };

}