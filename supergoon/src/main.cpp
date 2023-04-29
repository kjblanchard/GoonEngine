#include <Goon/scene/Scene.hpp>
#include <Goon/core/Application.hpp>
#include <Goon/systems/ScriptSystem.hpp>
#include <Supergoon/layers/EditorLayer.hpp>

int demo(goon::Scene &scene);
int main(int argc, char **argv)
{
    goon::Scene scene;
    scene.DeSerializeScene();

    // ScriptTesting
    auto domain = goon::ScriptSystem::InitializeMono();
    auto assembly = goon::ScriptSystem::OpenAssembly("hello.dll", domain);
    auto image = goon::ScriptSystem::OpenImage(assembly);
    auto class1 = goon::ScriptSystem::GetClassByName(image, "", "Class1");
    auto classInstance = goon::ScriptSystem::InstantiateClassObject(domain, class1);
    auto ctormethod = goon::ScriptSystem::GetConstructorInClass(class1);
    auto method = goon::ScriptSystem::GetMethodByName("PrintTest", "", class1);
    goon::ScriptSystem::CallMethod(ctormethod, classInstance);
    goon::ScriptSystem::CallMethod(method, classInstance);
    goon::ScriptSystem::CloseMono(domain);
    // EndScriptTesting

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
