#include <Goon/scene/Scene.hpp>
#include <Goon/core/Application.hpp>
#include <Goon/systems/ScriptSystem.hpp>
#include <Goon/core/Log.hpp>
#include <Supergoon/layers/EditorLayer.hpp>

//TODO make these better.
#include <GoonPlatforms/Window/SdlWindow.hpp>
#include <GoonPlatforms/Rendering/OpenGL/OpenGL.hpp>

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
    goon::Log::Init();
    GN_CORE_ERROR("What in the world is this {}" , 1);
    // EndScriptTesting

    demo(scene);
}

int demo(goon::Scene &scene)
{
    auto application = goon::Application();
    auto window = goon::WindowApi::Create();
    auto editor = goon::EditorLayer();
    editor.InitializeImGui();
    editor.LoadScene(scene);
    bool done = false;
    goon::OpenGL::ResizeWindow();

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
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(SDL_GL_GetCurrentWindow()))
                done = true;
        }
        editor.ImGuiNewFrame();
        editor.ProcessImGuiFrame();
        goon::OpenGL::StartDrawFrame();
        editor.DrawImGuiFrame();
        goon::OpenGL::EndDrawFrame();
    }
    editor.ExitImGui();

    return 0;
}
