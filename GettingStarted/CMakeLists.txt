cmake_minimum_required(VERSION 3.8.0)
project(GettingStarted VERSION 0.1.0)

set(SRC
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    )

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(GettingStarted ${SRC})

target_include_directories(GettingStarted PRIVATE 
    ${CMAKE_SOURCE_DIR}/includes
    ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(GettingStarted PRIVATE
    glfw
    imgui
    TracyClient
    glm
    ${GLEW_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
)