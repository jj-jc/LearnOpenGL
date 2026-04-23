cmake_minimum_required(VERSION 3.11.0)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
project(OpenGL VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.90.1
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
  
  add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  )
  target_include_directories(imgui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
  )
  target_link_libraries(imgui PUBLIC glfw)
endif()

FetchContent_Declare(
  tracy
  GIT_REPOSITORY https://github.com/wolfpld/tracy.git
  GIT_TAG        v0.11.1
)
set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy profiling" FORCE)
set(TRACY_ON_DEMAND ON CACHE BOOL "Enable on demand profiling" FORCE)
FetchContent_MakeAvailable(tracy)

# This defines an MY_DEBUG flag only if it is in debug mode
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG") 
# Set the libraries generated outside of the project inside the workspace
# set(CMAKE_LIBRAY_OUTPUT_DIRECTORY "/home/jjjurado/Dev/OpenGL") 
# message("${CMAKE_LIBRAY_OUTPUT_DIRECTORY}")
# option(MY_DEBUG "Defines MY_DEBUG option to debug output context in OpenGL" ON)
add_subdirectory(GettingStarted)
add_subdirectory(Tests)
add_subdirectory(Light)
add_subdirectory(ModelLoading)
add_subdirectory(TextRendering)
add_subdirectory(Breakout)
