cmake_minimum_required(VERSION 3.0)
project(src)

file(GLOB project_src CONFIGURE_DEPENDS "*.h" "*.cpp")

# include_directories(src PUBLIC "${PROJECT_SOURCE_DIR}")
add_executable(${PROJECT_NAME} ${project_src})
