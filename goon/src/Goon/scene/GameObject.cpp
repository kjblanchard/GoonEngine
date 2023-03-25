#include <Goon/scene/GameObject.hpp>
#include <Goon/scene/Scene.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/HierarchyComponent.hpp>

namespace goon
{

    GameObject::GameObject(entt::entity entityId, Scene *scene)
        : _entityId(entityId), _scene(scene)
    {
    }
    GameObject::~GameObject()
    {
    }
    void GameObject::AddChildEntity(entt::entity child)
    {
        auto childGameObject = GameObject{child, _scene};
        childGameObject.AddParentEntity(_entityId);
        auto &parentHierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        if (parentHierarchyComponent.FirstChild == entt::null)
        {
            parentHierarchyComponent.FirstChild = parentHierarchyComponent.NextChild = child;
            return;
        }

        auto &hierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        while (hierarchyComponent.NextChild != entt::null)
        {
            hierarchyComponent = _scene->Registry().get<HierarchyComponent>(hierarchyComponent.NextChild);
        }
        hierarchyComponent.NextChild = child;
    }

    void GameObject::AddParentEntity(entt::entity parent)
    {
        auto &hierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        hierarchyComponent.Parent = parent;
    }
}