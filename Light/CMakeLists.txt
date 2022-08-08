cmake_minimum_required(VERSION 3.0.0)
project(Light VERSION 0.1.0)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    )

# glew
# set(glew_DIR /usr/share/cmake-3.16/Modules/)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# OpenGL
find_package(OpenGL REQUIRED)
# include_directories(${OPENGL_gl_INCLUDE_DIRS})
link_libraries(${OPENGL_gl_LIBRARY})   

# GLFW
find_package(glfw3 REQUIRED)
link_libraries(glfw)

include_directories(${CMAKE_SOURCE_DIR}/includes)

add_executable(Light ${SRC})