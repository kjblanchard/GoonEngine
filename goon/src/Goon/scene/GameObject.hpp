#pragma once
#include <entt/entity/registry.hpp>
#include <Goon/scene/Scene.hpp>

namespace goon
{

    class GameObject
    {
    public:
        GameObject(entt::entity entityId, Scene *scene);
        ~GameObject();
        template <typename T, typename... Args>
        void AddComponent(Args &&...args)
        {
            _scene->Registry().emplace<T>(_entityId, std::forward<Args>(args)...);
        }
        template <typename T>
        T &GetComponent()
        {
            return _scene->Registry().get<T>(_entityId);
        }
        uint64_t GetID() const { return (uint64_t)_entityId; }
        template <typename T>
        void* GetComponentUniqueIntImGui()
        {
                return (void *)(intptr_t) (GetID() + typeid(T).hash_code() );

        }
        operator entt::entity() { return _entityId; }

        template <typename T>
        bool HasComponent()
        {
            return _scene->Registry().all_of<T>(_entityId);
        }

        /**
         * @brief Adds a child entity to this gameobject, as well as makes this gameobject his parent.
         *
         * @param child The child to add to this gameobjects hierarchy component
         */
        void AddChildEntity(entt::entity child);
        /**
         * @brief Adds a parent to this gameobject
         *
         * @param parent The new parent for this gameobject
         */
        void AddParentEntity(entt::entity parent);

    private:
        entt::entity _entityId;
        Scene *_scene;
    };

}