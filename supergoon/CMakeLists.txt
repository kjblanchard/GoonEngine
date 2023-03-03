# #########################################
# Supergoon Editor
# #########################################
message(STATUS "Creating Supergoon Editor executable..")
file(GLOB_RECURSE SUPERGOONFILES src/*.c*)

# IMGUI
add_subdirectory(external/imgui EXCLUDE_FROM_ALL)

# add_library(goonDll STATIC ${GOONDLLFILES})
add_executable(supergoon ${SUPERGOONFILES})
target_include_directories(supergoon
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
target_link_libraries(supergoon
        PUBLIC
        goonEngine
)
set_property(TARGET supergoon PROPERTY CXX_STANDARD 20)