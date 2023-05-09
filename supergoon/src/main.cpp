#include <Goon/scene/Scene.hpp>
#include <Goon/core/Application.hpp>
#include <Goon/systems/ScriptSystem.hpp>
#include <Goon/core/Log.hpp>
#include <Supergoon/layers/EditorLayer.hpp>

// TODO make these better.
#include <GoonPlatforms/Window/SdlWindowApi.hpp>
#include <GoonPlatforms/Rendering/OpenGL/OpenGL.hpp>
// Testing opengl
#include <Goon/Renderer/RendererAPI.hpp>
#include <Goon/Renderer/Buffer.hpp>
#include <Goon/Renderer/VertexArray.hpp>
#include <Goon/Renderer/Shader.hpp>

// testing loading image
#include <Goon/core/image_asset.hpp>
using namespace goon;

int demo(goon::Scene &scene);
std::shared_ptr<Shader> m_Shader;
std::shared_ptr<VertexArray> m_VertexArray;
std::shared_ptr<VertexArray> m_SquareVertexArray;
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
    GN_CORE_ERROR("What in the world is this {}", 1);
    // EndScriptTesting

    demo(scene);
}

int demo(goon::Scene &scene)
{
    auto application = goon::Application();
    // auto window = goon::WindowApi::Create();
    auto editor = goon::EditorLayer();
    editor.InitializeImGui();
    editor.LoadScene(scene);
    bool done = false;
    goon::OpenGL::ResizeWindow();
    m_VertexArray.reset(VertexArray::Create());

    // Triangle image
    auto asset = ImageAsset("assets/fun.jpg");
    asset.Load();

    // Basic triangle
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0, 1.0f, 1.0,
        -0.5f, 0.5f, 0.0f, 1.0f, 0, 1.0f, 1.0f};
    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    m_Shader.reset(Shader::Create("basic.glsl"));

    BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
    };
    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[] = {0, 2, 3, 1, 2, 3};
    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_SquareVertexArray.reset(VertexArray::Create());
    float squareVertices[] = {
        -0.75f, -0.75f, 0.0f, 0.0f, 1, 1.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 0.0f, 0, 1.0f, 1.0f,
        0.75f, 0.75f, 0.0f, 0.0f, 0, 1.0f, 1.0,
        -0.75f, 0.75f, 0.0f, 0.0f, 0, 1.0f, 1.0f};
    std::shared_ptr<VertexBuffer> squareVBO;
    squareVBO.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    BufferLayout squareLayout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
    };
    squareVBO->SetLayout(squareLayout);
    m_SquareVertexArray->AddVertexBuffer(squareVBO);
    unsigned int squareIndices[] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<IndexBuffer> squareIndexBuffer;
    squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

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
        m_Shader->Bind();

        OpenGL::Submit(m_SquareVertexArray);
        OpenGL::Submit(m_VertexArray);

        editor.DrawImGuiFrame();
        goon::OpenGL::EndDrawFrame();
    }
    editor.ExitImGui();

    return 0;
}
