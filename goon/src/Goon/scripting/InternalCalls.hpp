#pragma once

struct _MonoString;
typedef _MonoString MonoString;

namespace goon
{
    class InternalCalls
    {
    public:
        static uint64_t GameObject_GetGameObjectById(uint64_t entityId);
        static MonoString* GameObject_GetGameObjectName(uint64_t entityId);
    };
}
