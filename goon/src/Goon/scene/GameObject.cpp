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
    void GameObject::AppendChildEntity(entt::entity child)
    {
        auto childGameObject = GameObject{child, _scene};
        childGameObject.AddParentEntity(_entityId);
        auto &parentHierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        if (parentHierarchyComponent.FirstChild == entt::null)
        {
            parentHierarchyComponent.FirstChild = child;
            return;
        }

        auto nextChild = parentHierarchyComponent.FirstChild;
        while (true)
        {
            auto &hierarchy = _scene->Registry().get<HierarchyComponent>(nextChild);
            if (hierarchy.NextChild == entt::null)
                break;
            nextChild = hierarchy.NextChild;
        }
        auto &hierarchy = _scene->Registry().get<HierarchyComponent>(nextChild);
        hierarchy.NextChild = child;
    }

    void GameObject::AddParentEntity(entt::entity parent)
    {
        auto &hierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        hierarchyComponent.Parent = parent;
    }

    void GameObject::AddChildEntity(entt::entity child, entt::entity previousChild)
    {
        auto newChildGameObject = GameObject{child, _scene};
        auto &newChildHierarchy = newChildGameObject.GetComponent<HierarchyComponent>();
        auto &parentHierarchy = this->GetComponent<HierarchyComponent>();
        newChildGameObject.AddParentEntity(_entityId);
        if (previousChild == entt::null)
        {
            newChildHierarchy.NextChild = parentHierarchy.FirstChild;
            parentHierarchy.FirstChild = child;
            return;
        }
        else
        {
            auto nextIterChild = parentHierarchy.FirstChild;
            while (nextIterChild != entt::null)
            {
                auto nextChildGameObject = GameObject{nextIterChild, _scene};
                auto &nextChildHierarchy = nextChildGameObject.GetComponent<HierarchyComponent>();
                if (nextIterChild == previousChild)
                {
                    // This is the "previous child, so must add in."
                    newChildHierarchy.NextChild = nextChildHierarchy.NextChild;
                    nextChildHierarchy.NextChild = child;
                    break;
                }
                // Keep looking for next
                nextIterChild = nextChildHierarchy.NextChild;
            }
        }
    }
    void GameObject::RemoveChildEntity(entt::entity child)
    {
        auto newChildGameObject = GameObject{child, _scene};
        // auto &newChildHierarchy = newChildGameObject.GetComponent<HierarchyComponent>();
        auto &parentHierarchy = this->GetComponent<HierarchyComponent>();
        auto nextIterChild = parentHierarchy.FirstChild;
        if (parentHierarchy.FirstChild == child)
        {
            auto nextChildGameObject = GameObject{nextIterChild, _scene};
            auto &nextChildHierarchy = nextChildGameObject.GetComponent<HierarchyComponent>();
            parentHierarchy.FirstChild = nextChildHierarchy.NextChild;
        }
        else
        {
            while (nextIterChild != entt::null)
            {
                auto nextChildGameObject = GameObject{nextIterChild, _scene};
                auto &nextChildHierarchy = nextChildGameObject.GetComponent<HierarchyComponent>();
                if (nextChildHierarchy.NextChild == child)
                {
                    auto nextNextChildGameObject = GameObject{nextChildHierarchy.NextChild, _scene};
                    auto &nextNextChildHierarchy = nextNextChildGameObject.GetComponent<HierarchyComponent>();
                    // Handle replacing first child if it is the first child.
                    nextChildHierarchy.NextChild = nextNextChildHierarchy.NextChild;
                    // _scene->DestroyGameObject((uint64_t)nextChildHierarchy.NextChild);
                    // remove child
                }
                nextIterChild = nextChildHierarchy.NextChild;
            }
        }
    }
}