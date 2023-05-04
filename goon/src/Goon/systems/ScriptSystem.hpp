#pragma once
#include <mono/metadata/class.h>

namespace goon
{
    class Scene;
    class ScriptSystem
    {
    public:
        // TODO this should be moved.
        static Scene *scene;
        /**
         * @brief Initializes the JIT compiler, and sets the assemblies path for mono libs
         *
         * @return MonoDomain* The domain that mono is running in.
         */
        static MonoDomain *InitializeMono();
        /**
         * @brief Opens an assembly file, likely this needs to be converted into a Image afterwards for most inspection
         *
         * @param assemblyName The assembly name (file) to load
         * @param domain The domain to load this into
         * @return MonoAssembly* Loaded assembly
         */
        static MonoAssembly *OpenAssembly(std::string assemblyName, MonoDomain *domain);
        /**
         * @brief A monoimage is the CIL (common intermediate language) that can actually be invoked.
         *
         * @param assembly The Loaded assembly to read from
         * @return MonoImage* The mono Image that you can use to query things
         */
        static MonoImage *OpenImage(MonoAssembly *assembly);
        /**
         * @brief Returns a "class" that has information about this c# class by reading the CIL from the image.
         *
         * @param image The loaded image from the assembly (CIL)
         * @param name_space The namespace this class is in, leave empty for non-namespaced
         * @param classname The name of the class
         * @return MonoClass* Information about this c# class
         */
        static MonoClass *GetClassByName(MonoImage *image, std::string name_space, std::string classname);
        /**
         * @brief Looks through the mono class for a specific constructor, and returns the mono method handle to that object
         *
         * @param klass The monoclass to look through for this
         * @return MonoMethod* method handle for this constructor
         */
        static MonoMethod *GetConstructorInClass(MonoClass *klass);
        /**
         * @brief Looks through the klass monoclass for a specific method, and returns the mono method handle for that
         *
         * @param methodName The name of the method
         * @param name_space The namespace this is in
         * @param klass the monoclass to look through
         * @return MonoMethod* method handle for this method
         */
        static MonoMethod *GetMethodByName(std::string methodName, std::string name_space, MonoClass *klass);
        /**
         * @brief Allocate memory and Instantiate the klass monoclass and returns it.  This DOES NOT initialize the object,
         * but can be imagined as the first half of a new() call
         *
         * @param domain the monodomain to put this
         * @param klass the monoclass to create
         * @return MonoObject* the allocated class object
         */
        static MonoObject *InstantiateClassObject(MonoDomain *domain, MonoClass *klass);
        /**
         * @brief Calls a methodHandle monomethod on a instantiated monoclass objectInstance, and returns the result as a MonoObject
         *
         * @param methodHandle handle to the method to call
         * @param objectInstance instantiated object instance
         * @return MonoObject* an object* that contains the result of this function call.
         */
        static MonoObject *CallMethod(MonoMethod *methodHandle, void *objectInstance);
        /**
         * @brief Adds a unmanaged (C++) function to the monodomain
         *
         * @param name The name of the function that will be referenced inside of managed code (C#)
         * @param func The unmanaged (C++) function handle
         */
        static void AddInternalCall(std::string name, const void *func);
        /**
         * @brief Close mono gracefully
         *
         * @param domain The domain to shutdown.
         */
        static void CloseMono(MonoDomain *domain);


        static MonoDomain* domain;
    };
}