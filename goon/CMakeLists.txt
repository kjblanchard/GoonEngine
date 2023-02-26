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

# spdlog
add_subdirectory(external/spdlog EXCLUDE_FROM_ALL)

# SDL
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

# IMGUI
add_subdirectory(external/imgui EXCLUDE_FROM_ALL)

# entt
add_subdirectory(external/entt EXCLUDE_FROM_ALL)

# glm
add_subdirectory(external/glm EXCLUDE_FROM_ALL)

# goon_sound
add_subdirectory(external/goon_sound EXCLUDE_FROM_ALL)



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

        PRIVATE
)

target_include_directories(goonEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/

        PRIVATE
        external/imgui
        spdlog::spdlog
        external/glm/
        external/SDL
        external/goon_sound/src
        external/entt/src
)

# #########################################
# Install
# #########################################
# set(FOLDERS_TO_COPY shaders)
# install(DIRECTORY ${FOLDERS_TO_COPY}
# DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
# COMPONENT assets)