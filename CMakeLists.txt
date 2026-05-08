cmake_minimum_required(VERSION 3.20)
project(tmpfs-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)

add_executable(tmpfs-cpp)

target_sources(tmpfs-cpp PRIVATE src/main.cpp src/vnode.cpp)
