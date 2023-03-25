#include <iostream>
#include <memory>
#include <SDL.h>
#include <imgui.h>
#include <cstring>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <Goon/scene/Scene.hpp>
#include <Goon/scene/GameObject.hpp>
#include <Goon/core/bgm_asset.hpp>
#include <Goon/systems/SoundSystem.hpp>
#include <Goon/core/asset_manager.hpp>
#include <Goon/scene/components/BgmComponent.hpp>
#include <Goon/scene/components/TagComponent.hpp>
#include <Goon/scene/components/HierarchyComponent.hpp>
#include <Goon/scene/components/IdComponent.hpp>

int demo(goon::Scene &scene);

int main(int argc, char **argv)
{
    goon::Scene scene;
    // scene.DeSerializeScene();
    std::string name = "RootObject";
    auto rootObject = scene.CreateGameObject(name);
    name = "SmartCookie";
    auto boi = scene.CreateGameObject(name);
    rootObject.AddChildEntity(boi);
    name = "No u bro";
    auto boi2 = scene.CreateGameObject(name);
    rootObject.AddChildEntity(boi2);
    name = "No sound";
    auto boi3 = scene.CreateGameObject(name);
    rootObject.AddChildEntity(boi3);
    boi.AddComponent<goon::BgmComponent, std::string, float, float, bool>("./assets/menu1.ogg", 0, 3333, false);
    boi2.AddComponent<goon::BgmComponent, std::string, float, float, bool>("./assets/rain.ogg", 0, 10, true);
    scene.SerializeScene();
    demo(scene);
}

int demo(goon::Scene &scene)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Supergoon Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    // Setup SDL_Renderer instance
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != NULL);

    bool show_demo_window = true;
    // Main loop
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
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

// Start the Dear ImGui frame
// If Apple.
#ifdef GN_PLATFORM_MACOS
        int width, height, rwidth, rheight;
        float scalex, scaley;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetRendererOutputSize(renderer, &rwidth, &rheight);
        scalex = rwidth / width;
        scaley = rheight / height;
        SDL_RenderSetScale(renderer, scalex, scaley);
#endif

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::Text("Save");
                ImGui::Text("Load");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::Text("Hierarchy");
                ImGui::Text("Inspector");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ////
        // Hierarchy Panel
        ////
        ImGui::Begin("Hierarchy");
        static int entitySelected = -1;
        if (ImGui::TreeNode("Scene"))
        {

            scene.Registry().each([&](auto entityID)
                                  {
                goon::GameObject go{ entityID , &scene };
                auto& tagComponent = go.GetComponent<goon::TagComponent>();
                if (ImGui::Selectable(tagComponent,  go.GetID() == entitySelected))
                    entitySelected = go.GetID(); });
            ImGui::TreePop();
        }
        ImGui::End();

        ////
        // Inspector Panel
        ////
        ImGui::Begin("Inspector");
        if (entitySelected != -1)
        {
            goon::GameObject gameobject{(entt::entity)entitySelected, &scene};
            ImGui::Text("GameObject %s", gameobject.GetComponent<goon::TagComponent>().Tag.c_str());
            if (gameobject.HasComponent<goon::BgmComponent>())
            {
                if (ImGui::TreeNode((void *)(intptr_t)gameobject.GetID(), "Bgm Component "))
                {
                    auto &bgmComponent = gameobject.GetComponent<goon::BgmComponent>();
                    ImGui::Text("Filename:");
                    ImGui::SameLine();
                    char buffer[256];
                    memset(buffer, 0, sizeof(buffer));
                    snprintf(buffer, sizeof(buffer), "%s", bgmComponent.SoundFile.c_str());
                    if (ImGui::InputText("##Filename", buffer, sizeof(buffer)))
                    {

                        bgmComponent.UpdateSoundFileName(std::string(buffer));
                    }
                    ImGui::Text("Begin Point (s):");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##Begin", &bgmComponent.LoopBegin))
                    {
                        bgmComponent.UpdateLoopBegin(bgmComponent.LoopBegin);
                    }
                    ImGui::Text("End Point (s):");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##End", &bgmComponent.LoopEnd))
                    {
                        bgmComponent.UpdateLoopEnd(bgmComponent.LoopEnd);
                    }
                    ImGui::Text("Volume");
                    ImGui::SameLine();
                    ImGui::SliderFloat("##Volume", &bgmComponent.Volume, 0.0f, 1.0f);
                    if (ImGui::Button("Play"))
                    {
                        SSPlayBgm(bgmComponent.LoadedBgm.get()->_bgm, bgmComponent.Volume);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Pause"))
                    {
                        SSPauseBgm(bgmComponent.LoadedBgm.get()->_bgm);
                    }
                    ImGui::TreePop();
                }
            }
            if (gameobject.HasComponent<goon::HierarchyComponent>())
            {
                auto uniqueInt = gameobject.GetComponentUniqueIntImGui<goon::HierarchyComponent>();
                if (ImGui::TreeNode(uniqueInt, "HierarchyComponent"))
                {
                    auto &hierarchyComponent = gameobject.GetComponent<goon::HierarchyComponent>();
                    ImGui::Text("Parent: %d", hierarchyComponent.Parent);
                    ImGui::Text("NextChild: %d", hierarchyComponent.NextChild);
                    ImGui::Text("PrevChild: %d", hierarchyComponent.PreviousChild);
                    ImGui::Text("FirstChild: %d", hierarchyComponent.FirstChild);
                    ImGui::TreePop();
                }

            }
            if (gameobject.HasComponent<goon::IdComponent>())
            {
                auto uniqueInt = gameobject.GetComponentUniqueIntImGui<goon::IdComponent>();
                if (ImGui::TreeNode(uniqueInt, "IdInfo"))
                {
                    auto &idComponent = gameobject.GetComponent<goon::IdComponent>();
                    ImGui::Text("EntityId: %lld", gameobject.GetID());
                    uint64_t guid = idComponent.Guid;
                    ImGui::Text("Guid: %lld", guid);
                    ImGui::TreePop();
                }

            }
        }
        ImGui::End();

        // TODO - move this to our own renderer.
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(255), (Uint8)(255), (Uint8)(255), (Uint8)(255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
        // auto view = scene.Registry().view<goon::BgmComponent>();
        UpdateSoundBro();
        // for (auto entity : view)
        // {
        //     auto &bgm = view.get<goon::BgmComponent>(entity);
        //     bgm.LoadedBgm.get()->Update();
        // }
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
