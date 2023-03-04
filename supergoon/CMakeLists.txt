# #########################################
# Supergoon Editor
# #########################################
message(STATUS "Creating Supergoon Editor executable..")
file(GLOB_RECURSE SUPERGOONFILES src/*.c*)
add_executable(supergoon ${SUPERGOONFILES})

# Compile definitions.
if(APPLE)
        target_compile_definitions(supergoon PRIVATE -DGN_PLATFORM_MACOS)
elseif(WIN32)
        target_compile_definitions(supergoon PRIVATE -DGN_PLATFORM_WINDOWS)
elseif(LINUX)
        target_compile_definitions(supergoon PRIVATE -DGN_PLATFORM_LINUX)
endif(APPLE)

# IMGUI
add_subdirectory(external/imgui EXCLUDE_FROM_ALL)

# add_library(goonDll STATIC ${GOONDLLFILES})
target_include_directories(supergoon
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
target_link_libraries(supergoon
        PUBLIC
        goonEngine
)
set_property(TARGET supergoon PROPERTY CXX_STANDARD 20)