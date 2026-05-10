cmake_minimum_required(VERSION 3.20)
project(tmpfs-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)

option(BUILD_TESTING "Build the testing suite" ON)

add_library(tmpfs_lib STATIC)
target_sources(tmpfs_lib PRIVATE
    src/vnode.cpp
    src/filesystem.cpp
)
target_include_directories(tmpfs_lib PUBLIC src)

add_executable(tmpfs-cpp src/main.cpp)
target_link_libraries(tmpfs-cpp PRIVATE tmpfs_lib)

if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG        v1.14.0
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(fs_tests tests/fs_tests.cpp)
    target_link_libraries(fs_tests PRIVATE gtest_main tmpfs_lib)
endif()
