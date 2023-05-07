#include <Goon/scripting/InternalCalls.hpp>
#include <Goon/systems/ScriptSystem.hpp>
#include <Goon/scene/Scene.hpp>
#include <mono/metadata/object.h>
#include <Goon/scene/GameObject.hpp>

namespace goon
{

    uint64_t InternalCalls::GameObject_GetGameObjectById(uint64_t entityId)
    {
        Scene *scene = ScriptSystem::GetScene();
        auto thing = scene->GetGameObjectById(entityId);
        return thing.GetID();
    }
    MonoString *InternalCalls::GameObject_GetGameObjectName(uint64_t entityId)
    {
        Scene *scene = ScriptSystem::GetScene();
        auto gameobject = scene->GetGameObjectById(entityId);
        auto thing = mono_string_new(ScriptSystem::domain,gameobject.GetName());
        return thing;
    }
}