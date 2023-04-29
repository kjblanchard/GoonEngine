#include <Goon/systems/ScriptSystem.hpp>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
namespace goon
{
    MonoDomain *ScriptSystem::InitializeMono()
    {
        // Issues with terminal breaking mono console commands with segv in macos
        unsetenv("TERM");
        mono_set_assemblies_path("../lib/mono/lib");
        MonoDomain *domain;
        domain = mono_jit_init("hello");
        return domain;
    }

    MonoImage *ScriptSystem::OpenImage(MonoAssembly *assembly)
    {
        return mono_assembly_get_image(assembly);
    }

    MonoClass *ScriptSystem::GetClassByName(MonoImage *image, std::string name_space, std::string classname)
    {
        return mono_class_from_name(image, name_space.c_str(), classname.c_str());
    }

    MonoMethod *ScriptSystem::GetMethodByName(std::string methodName, std::string name_space, MonoClass *klass)
    {
        std::string name = mono_class_get_name(klass);
        // TODO add the namespace to the call if one is passed in.
        auto description = mono_method_desc_new(std::string(name + "::" + methodName + "()").c_str(), false);
        MonoMethod *method = mono_method_desc_search_in_class(description, klass);
        return method;
    }

    MonoObject *ScriptSystem::CallMethod(MonoMethod *methodHandle, void *objectInstance)
    {
        auto returnObj = mono_runtime_invoke(methodHandle, objectInstance, NULL, NULL);
        return returnObj;
    }

    MonoObject *ScriptSystem::InstantiateClassObject(MonoDomain *domain, MonoClass *klass)
    {
        MonoObject *my_class_instance = mono_object_new(domain, klass);
        return my_class_instance;
    }

    MonoMethod *ScriptSystem::GetConstructorInClass(MonoClass *klass)
    {
        MonoMethodDesc *desc = mono_method_desc_new(":.ctor()", 0);
        auto create_version = mono_method_desc_search_in_class(desc, klass);
        mono_method_desc_free(desc);
        return create_version;
    }
    void ScriptSystem::AddInternalCall(std::string name, const void *func)
    {
        mono_add_internal_call(std::string("Goon.InternalCalls::" + name).c_str(), func);
    }

    MonoAssembly *ScriptSystem::OpenAssembly(std::string assemblyName, MonoDomain *domain)
    {
        MonoAssembly *assembly;
        assembly = mono_domain_assembly_open(domain, assemblyName.c_str());
        if (!assembly)
        {
            printf("Assembly not loaded correctly!!");
            return nullptr;
        }
        return assembly;
    }

    void ScriptSystem::CloseMono(MonoDomain *domain)
    {
        mono_jit_cleanup(domain);
    }

}