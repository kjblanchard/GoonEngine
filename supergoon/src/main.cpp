#include <iostream>
#include <test.hpp>
#include <Goon/scene/Scene.hpp>


int main(int argc, char **argv)
{
    goon::Scene scene;
    std::string name = "Smart cookie";
    auto boi = scene.CreateGameObject(name);

    talk();
    demo();
}
