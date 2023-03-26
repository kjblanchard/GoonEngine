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
            printf("Root first child is %lld, and next child is ", child, child);
            return;
        }

        auto nextChild = parentHierarchyComponent.NextChild;
        while (true)
        {
            auto& hierarchy =  _scene->Registry().get<HierarchyComponent>(nextChild);
            if(hierarchy.NextChild == entt::null)
                break;
            nextChild = hierarchy.NextChild;
        }
        auto& hierarchy =  _scene->Registry().get<HierarchyComponent>(nextChild);
        hierarchy.NextChild = child;
    }

    void GameObject::AddParentEntity(entt::entity parent)
    {
        auto &hierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        hierarchyComponent.Parent = parent;
    }
}