cmake_minimum_required(VERSION 3.0)


# set build dir for debug and release configs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/release")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

file(GLOB project_src CONFIGURE_DEPENDS "*.h" "*.cpp")

add_executable( ${PROJECT_NAME} ${project_src} )

target_link_libraries( ${PROJECT_NAME} Vulkan::Vulkan glfw glm tinyobjloader stb_image imgui )
