cmake_minimum_required (VERSION 3.13)
project (Fortress)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
add_executable(fortress src/main.cpp)
add_library(fortresslib "")
target_link_libraries(fortress fortresslib)

# Add external projects from git submodules
add_subdirectory(third-party/soil)
target_include_directories(fortresslib PRIVATE third-party/soil/inc)
set(WITH_GFLAGS OFF CACHE BOOL "use gflags" FORCE)
add_subdirectory(third-party/glog)
#set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Enable testing" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Enable parse tools" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Enable contrib stuff in library" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "Enable generation of install target" FORCE)
add_subdirectory(third-party/yaml-cpp)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)
add_subdirectory(third-party/glfw)
set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" FORCE)
add_subdirectory(third-party/googletest)
add_subdirectory(third-party/lua)
set(SOL2_ENABLE_INSTALL OFF)
add_subdirectory(third-party/sol2)

# Ensure they are added to compiler and linker paths
target_link_libraries(fortresslib soil)
target_link_libraries(fortresslib glog::glog)
target_link_libraries(fortresslib yaml-cpp)
target_link_libraries(fortresslib glfw)
target_link_libraries(fortresslib lua_static)
target_link_libraries(fortresslib sol2)

# Add Compiler definitions
add_compile_definitions(FORTRESS_VERSION_MAJOR=0)
add_compile_definitions(FORTRESS_VERSION_MINOR=5)
add_compile_definitions(FORTRESS_VERSION_PATCH=0)
add_compile_definitions(FORTRESS_VERSION_BUILD="dev-build")

# Include source files
add_subdirectory(src/core)
add_subdirectory(src/algos)
add_subdirectory(src/components)
add_subdirectory(src/generators)
add_subdirectory(src/windows)
add_subdirectory(src/widgets)
add_subdirectory(src/systems)
add_subdirectory(src/world)

# Compiler options
target_compile_options(fortress PRIVATE -Wall -Wpedantic -Werror)

# Additional libraries
target_link_libraries(fortress dl)
target_link_libraries(fortress stdc++fs)
target_link_libraries(fortress GL)
target_link_libraries(fortress c)
target_link_libraries(fortress m)
target_link_libraries(fortress pthread)

# Add Testing target
enable_testing()
add_subdirectory(test/unit_tests)

# Add 'run' command
add_custom_target(run
    COMMAND ./fortress
    DEPENDS fortress
)