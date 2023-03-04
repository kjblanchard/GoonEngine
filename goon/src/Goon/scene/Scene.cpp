#include <Goon/scene/Scene.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Goon/scene/components/BgmComponent.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/TransformComponent.hpp>
#include <Goon/scene/components/IdComponent.hpp>
namespace goon
{
    Scene::Scene()
    {
    }

    GameObject Scene::CreateGameObject(std::string &name)
    {
        auto thing = _registry.create();
        _registry.emplace<TransformComponent>(thing);
        _registry.emplace<TagComponent>(thing, name);
        return GameObject(thing, this);
    }
}