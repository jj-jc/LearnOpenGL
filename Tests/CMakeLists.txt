cmake_minimum_required(VERSION 3.8.0)
project(Tests VERSION 0.1.0)

set(SRC
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    )

# glew
# find_package(GLEW REQUIRED)
# include_directories(${GLEW_INCLUDE_DIRS})
# link_libraries(${GLEW_LIBRARIES})
# set(GLEW_DIR C:/Dev/LearnOpenGL/Dependencies/glew-2.1.0)
# include_directories(${CMAKE_SOURCE_DIR}/Dependencies/glew-2.1.0/include/)
# link_libraries(${CMAKE_SOURCE_DIR}/Dependencies/glew-2.1.0/lib/Release/Win32/glew32.lib)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})


# OpenGL
find_package(OpenGL REQUIRED)
# include_directories(${OPENGL_gl_INCLUDE_DIRS})
link_libraries(${OPENGL_gl_LIBRARY})   

# GLFW
# find_package(glfw3 REQUIRED)
link_libraries(glfw)
# set(GLEW_DIR C:/Dev/LearnOpenGL/Dependencies/glfw-3.3.8)
# include_directories(${CMAKE_SOURCE_DIR}/Dependencies/glfw-3.3.8/include)
# link_libraries(${CMAKE_SOURCE_DIR}/Dependencies/glfw-3.3.8/lib/libglfw3.a)

# glm
include_directories(${glm_SOURCE_DIR})

# imgui
# include_directories(${CMAKE_SOURCE_DIR}/Dependencies/imgui/)


include_directories(${CMAKE_SOURCE_DIR}/includes)

add_executable(Tests ${SRC})