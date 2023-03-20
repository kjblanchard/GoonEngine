#include <Goon/scene/Scene.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Goon/scene/components/BgmComponent.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/TransformComponent.hpp>
#include <Goon/scene/components/IdComponent.hpp>

// yml / file operations
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>

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

    void Scene::SerializeScene()
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "sceneName";
        out << YAML::Value << "default";
        out << YAML::Key << "gameobjects";
        out << YAML::Value << YAML::BeginSeq;
        _registry.each([&](auto entityID)
                       {
                goon::GameObject go { entityID , this };
                auto& tagComponent = go.GetComponent<goon::TagComponent>();
                auto& idComponent = go.GetComponent<goon::IdComponent>();
                uint64_t id = idComponent.Guid;
                out << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "name";
                out << YAML::Value << tagComponent;
                out << YAML::Key << "id";
                out << YAML::Value << id;
                out << YAML::Key << "components";
                out << YAML::Value << YAML::BeginSeq;
                if(go.HasComponent<goon::BgmComponent>())
                {
                    auto& bgmComponent = go.GetComponent<goon::BgmComponent>();
                    out << YAML::BeginMap;
                    out << YAML::Key << "bgm";
                    out << YAML::Value << YAML::BeginMap;
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
                    out << YAML::EndMap << YAML::EndMap;
                }
                // out << YAML::EndSeq <<  YAML::EndMap << YAML::EndMap; });
                out << YAML::EndSeq  << YAML::EndMap; });
        out << YAML::EndSeq;
        out << YAML::EndMap;
        std::cout << out.c_str() << std::endl;
        std::ofstream outFile;
        outFile.open("assets/default.yml", std::ios::trunc);
        outFile << out.c_str();
    }

    void Scene::DeSerializeScene()
    {
        YAML::Node config = YAML::LoadFile("assets/default.yml");
        auto sceneName = config["sceneName"].as<std::string>();
        printf("Name is %s", sceneName.c_str());
        auto gameobjects = config["gameobjects"];
        if(gameobjects)
        {
            for(auto gameobject: gameobjects)
            {
                auto name = gameobject["name"].as<std::string>();
                auto id = gameobject["id"].as<uint64_t>();
                printf("Object: name %s id %ull", name.c_str(), id);
            }
        }
    }

    GameObject Scene::CreateGameObject(std::string &name)
    {
        auto thing = _registry.create();
        _registry.emplace<TransformComponent>(thing);
        _registry.emplace<TagComponent>(thing, name);
        _registry.emplace<IdComponent>(thing);
        return GameObject(thing, this);
    }

}