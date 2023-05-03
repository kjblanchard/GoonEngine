#pragma once
#include <entt/entity/registry.hpp>
#include <Goon/scene/Scene.hpp>

namespace goon
{

    /**
     * @brief convienence Wrapper arount an Entt::Entity, used mainly for it's functions.
     * Can upscale a entity into a gameobject by passing in scene, and size is still uintt64.
     */
    class GameObject
    {
    public:
        GameObject(entt::entity entityId, Scene *scene);
        uint64_t GetID() const { return (uint64_t)_entityId; }
        entt::entity GetEntity() const { return _entityId; }
        /**
         * @brief Gets the name of the gameobject.
         *
         * @return const char* Name
         */
        const char *GetName();
        /**
         * @brief Adds a component to this gameobject
         *
         * @tparam T The component you wish to add
         * @tparam Args The components ctor argument types
         * @param args The components ctor arguments
         */
        template <typename T, typename... Args>
        void AddComponent(Args &&...args)
        {
            _scene->Registry().emplace<T>(_entityId, std::forward<Args>(args)...);
        }
        /**
         * @brief Gets a Component from this gameobject.  This is unsafe and should be ran after HasComponent returns true, else exception.
         *
         * @tparam T The component you are looking for
         * @return T& Component
         */
        template <typename T>
        T &GetComponent()
        {
            return _scene->Registry().get<T>(_entityId);
        }
        /**
         * @brief Checks to see if this gameobject has a component
         *
         * @tparam T The component to check for
         * @return true If the component exists on the gameobject
         * @return false If the component does not exist on the gameobject
         */
        template <typename T>
        bool HasComponent()
        {
            return _scene->Registry().all_of<T>(_entityId);
        }
        /**
         * @brief Removes a component from a gameobject
         *
         * @tparam T The component type to remove.
         */
        template <typename T>
        void RemoveComponent()
        {
            if (!HasComponent<T>())
                return;
            _scene->Registry().remove<T>(_entityId);
        }

    public:
        operator entt::entity() { return _entityId; }
        void AddChildEntity(entt::entity child, entt::entity previousChild);
        void RemoveChildEntity(entt::entity child);
        /**
         * @brief Adds a child entity to this gameobject, as well as makes this gameobject his parent.
         *
         * @param child The child to add to this gameobjects hierarchy component
         */
        void AppendChildEntity(entt::entity child);
        void UpdatePreviousChild(uint64_t previousChild);
        void UpdatePreviousChild(entt::entity previousChild);

    private:
        entt::entity _entityId;
        Scene *_scene;
        /**
         * @brief Adds a parent to this gameobject
         *
         * @param parent The new parent for this gameobject
         */
        void AddParentEntity(entt::entity parent);

    public:
        // TODO move these out of gameobject
        /**
         * @brief Used for generating a unique int from this gameobject
         *
         * @return void*
         */
        void *GetGameobjectUniqueIntImgui()
        {
            return (void *)(intptr_t)(GetID() + typeid(GameObject).hash_code());
        }
        /**
         * @brief Used for generating a unique int from the type sent in.  Probably should combine the two or remove.  This is used in editor for imgui, likely move it there..
         *
         * @tparam T
         * @return void*
         */
        template <typename T>
        void *GetComponentUniqueIntImGui()
        {
            return (void *)(intptr_t)(GetID() + typeid(T).hash_code());
        }
    };

}