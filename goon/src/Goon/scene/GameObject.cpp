#include <Goon/scene/GameObject.hpp>
#include <Goon/scene/Scene.hpp>
#include <Goon/scene/components/TagComponent.hpp>

namespace goon
{

    GameObject::GameObject(entt::entity entityId, Scene* scene)
    : _entityId(entityId), _scene(scene)
    {
        auto& tagComponent = _scene->Registry().get<TagComponent>(entityId);
        printf("This guys name is %s", tagComponent.Tag.c_str());
    }
    GameObject::~GameObject()
    {
    }
}