#include <Goon/scene/Scene.hpp>
#include <Goon/core/Application.hpp>
#include <Supergoon/layers/EditorLayer.hpp>

// mono testing
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

int demo(goon::Scene &scene);
int main(int argc, char **argv)
{
    // Issues with terminal breaking mono console commands, so this term is needed.
    unsetenv("TERM");
    goon::Scene scene;
    scene.DeSerializeScene();
    mono_set_assemblies_path("../lib/mono/lib");
    MonoDomain *domain;
    domain = mono_jit_init("hello");
    MonoAssembly *assembly;
    assembly = mono_domain_assembly_open(domain, "hello.dll");
    if (!assembly)
        return 99;
    MonoImage *image = mono_assembly_get_image(assembly);
    MonoClass *klass = mono_class_from_name(image, "", "Class1");
    MonoMethodDesc *ctorDesc = mono_method_desc_new("Class1:Class1()", false);  // works
    // MonoMethod *ctorMethod = mono_method_desc_search_in_class(ctorDesc, klass); // fails because klass is nullptr

    MonoMethodDesc *doDesc = mono_method_desc_new("Class1::PrintTest()", false); // works
    MonoMethod *doMethod = mono_method_desc_search_in_class(doDesc, klass);      // fails because klass is nullptr
    /* allocate memory for the object */
    MonoObject *my_class_instance = mono_object_new(domain, klass);
    /* execute the default argument-less constructor */
    mono_runtime_object_init(my_class_instance);
    mono_runtime_invoke(doMethod, my_class_instance, NULL, NULL);
    mono_jit_cleanup(domain);
    demo(scene);
}

int demo(goon::Scene &scene)
{
    auto application = goon::Application();
    application.InitializeSDL();
    auto editor = goon::EditorLayer();
    editor.InitializeImGui();
    editor.LoadScene(scene);
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            editor.ProcessImGuiEvents(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(application.GetWindow()))
                done = true;
        }
        application.ResizeWindow();
        editor.ImGuiNewFrame();
        editor.ProcessImGuiFrame(scene);
        application.StartDrawFrame();
        editor.DrawImGuiFrame();
        application.EndDrawFrame();
        // TODO reenable sound, or put into a system.
        // UpdateSoundBro();
    }

    editor.ExitImGui();
    application.DestroyWindow();
    application.ExitSdl();

    return 0;
}
