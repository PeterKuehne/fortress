add_executable(fortress main.cpp)

# Create the basic library
add_library(fortresslib "")
target_link_libraries(fortress fortresslib)

# Include source files
add_subdirectory(core)
add_subdirectory(algos)
add_subdirectory(components)
add_subdirectory(generators)
add_subdirectory(windows)
add_subdirectory(widgets)
add_subdirectory(systems)
add_subdirectory(world)

# SOIL
target_include_directories(fortresslib PRIVATE ${soil_SOURCE_DIR}/inc)
if(MINGW)
  target_link_libraries(soil opengl32)
else()
  target_link_libraries(soil GL)
endif(MINGW)

target_link_libraries(fortresslib lua_static)
target_link_libraries(fortresslib glfw)
target_link_libraries(fortresslib glog::glog)
target_link_libraries(fortresslib soil)
target_link_libraries(fortresslib yaml-cpp)

# Add Compiler definitions
add_compile_definitions(FORTRESS_VERSION_MAJOR=0)
add_compile_definitions(FORTRESS_VERSION_MINOR=5)
add_compile_definitions(FORTRESS_VERSION_PATCH=0)
add_compile_definitions(FORTRESS_VERSION_BUILD="dev-build")

# Compiler options
target_compile_options(fortress PRIVATE -Wall -Wpedantic -Werror)

# Additional libraries
target_link_libraries(fortress dl)
target_link_libraries(fortress stdc++fs)
target_link_libraries(fortress m)
target_link_libraries(fortress pthread)
if(MINGW)
  target_link_libraries(fortress opengl32)
  target_link_options(fortress PRIVATE -static)
else()
  target_link_libraries(fortress GL)
endif(MINGW)
