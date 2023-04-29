# Goon Engine
A C++ engine with OpenGL rendering and C# (mono) scripting, using Dear ImGui as an editor layer.  Starting as a project to practice c++ and embed c#, and to learn basics of OpenGL

![Picture didn't load idiot](https://github.com/kjblanchard/GoonEngine/blob/master/img/new.gif?raw=true)

## Current State
- A simple editor layer, with basic components such as Name, Hierarchy (For parenting), and bgm.
- BGM component uses goon_sound (OpenAL) and can loop with introduction.  Meaning it takes two loop points, and will contiually play between those loop points after starting from the beginning
- Scripting work has started, mono is embedded but no script components are created.
- Rendering work has not started, and currently using ImGuis demo backend, after script component Rendering will be started.

## Requirements
- Cmake will install all of the required libraries (mostly by git submodule initiazation) except for mono
- Mono must already be built and cmake will search for it, currently Mono is not easily embeddable in cmake as it requires itself to be built, and we would need to create a cmakefile for it.

## Components
- CMake: The actual cross-platform build system.
- SDL2: Low Level handling of windowing, events, input
- OpenAL: Used with goon_sound for cross-platform 3D sound
- OpenGL: Rendering Backend
- Mono: C# (.NET framework) scripting
- Dear ImGui: Editor
- EnTT: Entity Component System
- Make: Streamlining building with one command

## Licenses
- LGPLv2: Due to OpenALsoft