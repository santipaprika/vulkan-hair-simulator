cmake_minimum_required(VERSION 3.0)
project(vulkan-renderer)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED COMPONENTS glslc)

add_subdirectory(src)
add_subdirectory(libs)
add_subdirectory(shaders)