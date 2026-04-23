cmake_minimum_required(VERSION 3.8.0)
project(GettingStarted VERSION 0.1.0)

set(SRC
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    )

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# OpenGL
find_package(OpenGL REQUIRED)
link_libraries(${OPENGL_gl_LIBRARY})   

# GLFW
link_libraries(glfw)

include_directories(${CMAKE_SOURCE_DIR}/includes)

add_executable(GettingStarted ${SRC})