cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting Goon Engine configuration")

# #########################################
# File globbing
# #########################################
message(STATUS "Getting all files for cmake..")
file(GLOB_RECURSE GOONENGINEFILES src/*.c* src/*.h*)
list(REMOVE_ITEM GOONENGINEFILES "main.cpp")

# #########################################
# Platform specific Project setup
# #########################################
message(STATUS "Changing the platform specific settings")

if(APPLE)
        # MACOS
        if(NOT IOS)
                set(CMAKE_THREAD_LIBS_INIT "-lpthread") # We need to say that we have threads, or else borked for some reason.
                set(CMAKE_HAVE_THREADS_LIBRARY 1)
                set(CMAKE_USE_WIN32_THREADS_INIT 0)
                set(CMAKE_USE_PTHREADS_INIT 1)
                set(THREADS_PREFER_PTHREAD_FLAG ON)
        endif(NOT IOS)
endif(APPLE)

if(WIN32)
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /c/cmake)
endif(WIN32)

# #########################################
# Initialize
# #########################################
message(STATUS "Initializing the project")
include(FindPkgConfig)
add_library(goonEngine SHARED ${GOONENGINEFILES})
set_property(TARGET goonEngine PROPERTY CXX_STANDARD 20)

if(UNIX AND NOT APPLE)
        set(LINUX ON)
endif(UNIX AND NOT APPLE)

# #########################################
# Options
# #########################################
option(GOON_GIT_SUBMODULE "Check submodules during build" ON)
option(GOON_BUILD_PCH "Uses a PCH file to try and speed up compilation" ON)
option(GOON_BUILD_ENGINE "Builds the full engine into the project" ON)
option(CMAKE_DEBUG_VARIABLES "Runs a debug on all variables for troubleshooting" ON)

# Not implemented.
option(GOON_INSTALL_SUBMODULES "Install the submodules" ON)

# #########################################
# Build libraries
# #########################################
message(STATUS "Building libraries")

# Update submodules
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        if(GOON_GIT_SUBMODULE)
                message(STATUS "Updating git submodules")
                execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)

                if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
                endif()
        endif()
endif()

# #########################################
# Build dependencies
# #########################################
# spdlog
add_subdirectory(external/spdlog EXCLUDE_FROM_ALL)

# SDL
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

# entt
add_subdirectory(external/entt EXCLUDE_FROM_ALL)

# glm
add_subdirectory(external/glm EXCLUDE_FROM_ALL)

# goon_sound
add_subdirectory(external/goon_sound EXCLUDE_FROM_ALL)

# yaml
add_subdirectory(external/yaml-cpp EXCLUDE_FROM_ALL)

# mono
pkg_check_modules(mono-2 REQUIRED mono-2)

# #########################################
# For Debug, print aout all variables.
# #########################################
if(CMAKE_DEBUG_VARIABLES)
        message(STATUS "Exporting debug")
        get_cmake_property(_variableNames VARIABLES)
        list(SORT _variableNames)

        foreach(_variableName ${_variableNames})
                message(STATUS "${_variableName}=${${_variableName}}")
        endforeach()
endif(CMAKE_DEBUG_VARIABLES)

# #########################################
# Precompiled headers
# #########################################
if(GOON_BUILD_PCH)
        message(STATUS "Setting up PCH, mac needs objc")
        target_precompile_headers(goonEngine PRIVATE
                "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/gnpch.hpp>")
endif(GOON_BUILD_PCH)

# #########################################
# Compile Definitions
# #########################################
target_compile_definitions(goonEngine PRIVATE)

if(GOON_BUILD_ENGINE)
        target_compile_definitions(goonEngine PRIVATE

                # -DGN_BUILD_DLL # Builds the full project
                # -DGN_ENABLE_ASSERTS # Enables asserts
        )
endif(GOON_BUILD_ENGINE)

if(APPLE)
        target_compile_definitions(goonEngine PRIVATE -DGN_PLATFORM_MACOS)
elseif(WIN32)
        target_compile_definitions(goonEngine PRIVATE -DGN_PLATFORM_WINDOWS)
elseif(LINUX)
        target_compile_definitions(goonEngine PRIVATE -DGN_PLATFORM_LINUX)
endif(APPLE)

# #########################################
# Compile Options
# #########################################
target_compile_options(goonEngine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wdeprecated
        -Wextra-semi
)

# #########################################
# Linking Options
# #########################################
target_link_libraries(goonEngine
        PUBLIC
        spdlog::spdlog
        imgui
        SDL2::SDL2
        EnTT::EnTT
        goon_sound
        yaml-cpp
        ${mono-2_LDFLAGS}

        PRIVATE
)

target_include_directories(goonEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        external/goon_sound/src
        ${mono-2_INCLUDEDIR}

        PRIVATE
        external/imgui
        spdlog::spdlog
        external/glm/
        external/SDL
        external/entt/src
        external/yaml-cpp/include
)

# #########################################
# Install
# #########################################

# MACOS SPECIFIC CURRENTLY FOR MONO, this likely should be done differently, as a install target, or something.  Read the docs.
set(FOLDERS_TO_COPY /opt/homebrew/lib/mono/4.5)
# Copy mono lib folder over, this is needed for embedding mono
install(DIRECTORY ${FOLDERS_TO_COPY}
        DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/mono/lib
        COMPONENT mono)

# Copy the built dylib for mono over, as this cannot be built in cmake and should be built locally.
install(FILES /opt/homebrew/lib/libmono-2.0.dylib
        DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

# Copy the template file over until we automate this in the engine
install(FILES /Users/kevin/git/engine/goon/supergoon/template.yml
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        RENAME default.yml
)
