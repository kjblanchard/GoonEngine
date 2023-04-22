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
        auto &childHierarchy = _scene->Registry().get<HierarchyComponent>(child);
        if (parentHierarchyComponent.FirstChild == entt::null)
        {
            // Add at beginning
            AddChildEntity(child, entt::null);
        }
        else
        {
            // Go to end, and then add it there.
            auto nextChild = parentHierarchyComponent.FirstChild;
            while (true)
            {
                auto &hierarchy = _scene->Registry().get<HierarchyComponent>(nextChild);
                if (hierarchy.NextChild == entt::null)
                    break;
                nextChild = hierarchy.NextChild;
            }
            AddChildEntity(child, nextChild);
        }
        childHierarchy.NextChild = entt::null;
    }

    void GameObject::AddParentEntity(entt::entity parent)
    {
        auto &hierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        hierarchyComponent.Parent = parent;
    }
    void GameObject::UpdatePreviousChild(uint64_t previousChild)
    {
        UpdatePreviousChild((entt::entity)previousChild);
    }

    void GameObject::UpdatePreviousChild(entt::entity previousChild)
    {
        auto &hierarchyComponent = _scene->Registry().get<HierarchyComponent>(_entityId);
        hierarchyComponent.PreviousChild = previousChild;
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
            newChildHierarchy.PreviousChild = entt::null;

            // Handle if the parent already has a first child
            if (parentHierarchy.FirstChild != entt::null)
            {
                auto nextChildGameObject = GameObject{parentHierarchy.FirstChild, _scene};
                auto &nextChildHierarchy = nextChildGameObject.GetComponent<HierarchyComponent>();
                nextChildHierarchy.PreviousChild = child;
            }
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
                    // Get nextnext, so that we can update his stuff if he exists.
                    auto nextNextChild = nextChildHierarchy.NextChild;
                    if (nextNextChild != entt::null)
                    {
                        auto nextNextChildGameObject = GameObject{nextChildHierarchy.NextChild, _scene};
                        auto &nextNextChildHierarchy = nextNextChildGameObject.GetComponent<HierarchyComponent>();
                        nextNextChildHierarchy.PreviousChild = child;
                    }
                    newChildHierarchy.NextChild = nextNextChild;
                    newChildHierarchy.PreviousChild = previousChild;
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
                    nextChildHierarchy.NextChild = nextNextChildHierarchy.NextChild;
                    nextNextChildHierarchy.PreviousChild = nextIterChild;
                }
                nextIterChild = nextChildHierarchy.NextChild;
            }
        }
    }
}