FetchContent_Declare(
	SwearJar
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        master
	)
FetchContent_MakeAvailable(SwearJar)

add_executable(unit_test test_main.cpp)
target_link_libraries(unit_test fortresslib)

target_include_directories(unit_test PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_sources (unit_test 
    PRIVATE
        mocks/component_manager_mock.h
        mocks/entity_manager_mock.h
        mocks/game_engine_mock.h
        mocks/graphics_mock.h
        mocks/grouping_manager_mock.h
        mocks/map_manager_mock.h
        mocks/widget_mock.h
        mocks/window_manager_mock.h
        mocks/world_info_mock.h
        tests/combat_system_test.cpp
        tests/component_manager_test.cpp
        tests/component_tests.cpp
        tests/dialog_system_test.cpp
        tests/entity_manager_test.cpp
        tests/frame_test.cpp
        tests/grouping_manager_test.cpp
        tests/grouping_test.cpp
        tests/icon_test.cpp
        tests/interaction_system_test.cpp
        tests/label_test.cpp
        tests/listbox_test.cpp
        tests/location_test.cpp
        tests/map_manager_test.cpp
        tests/npc_system_test.cpp
        tests/numeric_entry_test.cpp
        tests/progress_bar_test.cpp
        tests/room_test.cpp
        tests/tab_test.cpp
        tests/text_block_test.cpp
        tests/text_entry_test.cpp
        tests/tile_test.cpp
        tests/widget_test.cpp
        tests/world_info_test.cpp
    )

target_link_libraries(unit_test soil)
target_link_libraries(unit_test glog::glog)
target_link_libraries(unit_test yaml-cpp)
target_link_libraries(unit_test glfw)
target_link_libraries(unit_test gtest gmock)
target_link_libraries(unit_test lua_static)

target_link_libraries(unit_test dl)
target_link_libraries(unit_test stdc++fs)
target_link_libraries(unit_test m)
target_link_libraries(unit_test pthread)
if(MINGW)
  target_link_libraries(unit_test opengl32)
else()
  target_link_libraries(unit_test GL)
endif(MINGW)

add_test (
    NAME unit_test
    COMMAND unit_test --gtest_shuffle
)