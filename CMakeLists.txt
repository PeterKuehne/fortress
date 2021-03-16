cmake_minimum_required (VERSION 3.13)
project (Fortress)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

# Make dependencies available
FetchContent_Declare(
	lua
	GIT_REPOSITORY https://github.com/walterschell/lua.git
	GIT_TAG        master
)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG        master
)
FetchContent_Declare(
	glog
	GIT_REPOSITORY https://github.com/google/glog.git
	GIT_TAG        master
)
FetchContent_Declare(
  soil
  GIT_REPOSITORY https://github.com/paralin/soil.git
  GIT_TAG        master
)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        master
)
  
FetchContent_MakeAvailable(lua glfw glog soil yaml-cpp)

# Add sources
add_subdirectory(src)

# Add Testing target
enable_testing()
add_subdirectory(test/unit_tests)

# Add 'run' command
add_custom_target(run
    COMMAND ./fortress
    DEPENDS fortress
)