#include <Goon/scene/Scene.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Goon/scene/components/BgmComponent.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/TransformComponent.hpp>
#include <Goon/scene/components/IdComponent.hpp>
#include <Goon/scene/components/HierarchyComponent.hpp>
#include <Goon/scene/components/InactiveComponent.hpp>

// yml / file operations
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>

// static entt::entity CreateGameObjectFromYaml(uint64_t entityId, YAML::Node &gameObjectNode);
namespace goon
{

    ////////////////////
    // Yml emitters
    ////////////////////
    std::ostream &operator<<(std::ostream &out, const TagComponent &tagComp)
    {
        out << tagComp.Tag;
        return out;
    }
    YAML::Emitter &operator<<(YAML::Emitter &out, const TagComponent &tagComp)
    {
        out << tagComp.Tag;
        return out;
    }

    Scene::Scene()
    {
    }
    Guid GetGuidOfEntity(entt::entity entity, Scene &scene);

    void Scene::SerializeScene()
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "sceneName";
        out << YAML::Value << _sceneName;
        out << YAML::Key << "rootObject";
        out << YAML::Value << GetGuidOfEntity(RootObject, *this);
        out << YAML::Key << "gameobjects";
        out << YAML::Value << YAML::BeginMap;
        _registry.each([&](auto entityID)
                       {
                goon::GameObject go { entityID , this };
                if(!go.HasComponent<InactiveComponent>())
                {
                auto& tagComponent = go.GetComponent<goon::TagComponent>();
                auto& idComponent = go.GetComponent<goon::IdComponent>();
                uint64_t id = idComponent.Guid;
                out << YAML::Key << id;
                out << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "name";
                out << YAML::Value << tagComponent;
                out << YAML::Key << "components";
                out << YAML::Value << YAML::BeginSeq;
                if(go.HasComponent<goon::BgmComponent>())
                {
                    auto& bgmComponent = go.GetComponent<goon::BgmComponent>();
                    // make the object
                    out << YAML::BeginMap;
                    out << YAML::Key << "componentName";
                    out << YAML::Value << "bgm";
                    out << YAML::Key << "fileName";
                    out << YAML::Value << bgmComponent.SoundFile;
                    out << YAML::Key << "loopBegin";
                    out << YAML::Value << bgmComponent.LoopBegin;
                    out << YAML::Key << "loopEnd";
                    out << YAML::Value << bgmComponent.LoopEnd;
                    out << YAML::Key << "ambientSound";
                    out << YAML::Value << bgmComponent.Ambient;
                    out << YAML::Key << "volume";
                    out << YAML::Value << bgmComponent.Volume;
                    out << YAML::EndMap;
                }
                if(go.HasComponent<goon::HierarchyComponent>())
                {
                    auto& hierarchyComponent = go.GetComponent<goon::HierarchyComponent>();
                    out << YAML::BeginMap;
                    out << YAML::Key << "componentName";
                    out << YAML::Value << "hierarchy";
                    out << YAML::Key << "firstChild";
                    out << YAML::Value << GetGuidOfEntity(hierarchyComponent.FirstChild, *this);
                    out << YAML::Key << "nextChild";
                    out << YAML::Value << GetGuidOfEntity(hierarchyComponent.NextChild, *this);
                    out << YAML::Key << "parent";
                    out << YAML::Value << GetGuidOfEntity(hierarchyComponent.Parent, *this);
                    out << YAML::EndMap;
                }
                out << YAML::EndSeq  << YAML::EndMap;

                } });
        out << YAML::EndMap;
        out << YAML::EndMap;
        std::cout << out.c_str() << std::endl;
        std::ofstream outFile;
        outFile.open("assets/" + _sceneName + ".yml", std::ios::trunc);
        outFile << out.c_str();
    }

    Guid GetGuidOfEntity(entt::entity entity, Scene &scene)
    {
        if (entity == entt::null)
            return 0;
        goon::GameObject go{entity, &scene};
        auto &thing = go.GetComponent<IdComponent>();
        return thing.Guid;
    }

    void Scene::DeSerializeScene()
    {
        YAML::Node config = YAML::LoadFile("assets/" + _sceneName + ".yml");
        auto sceneName = config["sceneName"].as<std::string>();
        auto rootObject = Guid(config["rootObject"].as<uint64_t>());
        auto gameobjects = config["gameobjects"];
        // TODO replace the exit
        if (!gameobjects)
            exit(1);
        CreateGameObjectFromYaml(rootObject, entt::null, gameobjects);
    }

    GameObject Scene::CreateGameObject(std::string &name, entt::entity parent)
    {
        auto thing = _registry.create();
        _registry.emplace<TransformComponent>(thing);
        _registry.emplace<TagComponent>(thing, name);
        _registry.emplace<IdComponent>(thing);
        _registry.emplace<HierarchyComponent>(thing);
        auto guy = GameObject(thing, this);
        if (parent != entt::null)
        {
            auto parentGo = GameObject(parent, this);
            parentGo.AddChildEntity(guy);
        }
        return guy;
    }

    entt::entity Scene::CreateGameObjectFromYaml(uint64_t entityId, entt::entity parent, YAML::Node &gameObjectNode)
    {
        auto goData = gameObjectNode[entityId];
        auto goEntity = _registry.create();
        _registry.emplace<TransformComponent>(goEntity);
        auto name = goData["name"].as<std::string>();
        _registry.emplace<TagComponent>(goEntity, name);
        _registry.emplace<IdComponent>(goEntity, entityId);
        _registry.emplace<HierarchyComponent>(goEntity);
        GameObject go = {goEntity, this};
        auto components = goData["components"];
        for (auto component : components)
        {
            auto componentName = component["componentName"].as<std::string>();
            // Try and create all of the component types.
            // BgmComponent(std::string soundFile, float loopBegin = 0.0f, float loopEnd = 0.0f, bool bg = false, float volume = 1.0f)
            if (componentName == "bgm")
            {
                auto fileName = component["fileName"].as<std::string>();
                auto loopBegin = component["loopBegin"].as<float>();
                auto loopEnd = component["loopEnd"].as<float>();
                auto ambient = component["ambientSound"].as<bool>();
                auto volume = component["volume"].as<int>();
                _registry.emplace<BgmComponent>(goEntity, fileName, loopBegin, loopEnd, ambient, volume);
                // bgmComponent.
            }
            else if (componentName == "hierarchy")
            {
                // If hierarchy.nextchild, nextchild = recursive
                // _registry.emplace<HierarchyComponent>(goEntity);
                auto firstChild = component["firstChild"].as<uint64_t>();
                auto nextChild = component["nextChild"].as<uint64_t>();
                // auto parent = component["parent"].as<uint64_t>();
                auto &hierarchy = go.GetComponent<goon::HierarchyComponent>();
                hierarchy.Parent = parent;
                if (firstChild)
                {
                    auto &hierarchy = go.GetComponent<goon::HierarchyComponent>();
                    hierarchy.FirstChild = CreateGameObjectFromYaml(firstChild, goEntity, gameObjectNode);
                }
                if (nextChild)
                {
                    hierarchy.NextChild = CreateGameObjectFromYaml(nextChild, goEntity, gameObjectNode);
                }
                // If hierarchy.firstchild, firstchild = Recursive
            }
        }
        return goEntity;
        // end
    }

    void Scene::DestroyGameObject(uint64_t entityId)
    {
        auto entity = (entt::entity)entityId;
        _registry.emplace<InactiveComponent>(entity);
    }

}
