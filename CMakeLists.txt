cmake_minimum_required(VERSION 3.0.0)
project(OpenGL)
set(CMAKE_CXX_STANDARD 17)
# This defines an MY_DEBUG flag only if it is in debug mode
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG") 
# Set the libraries generated outside of the project inside the workspace
# set(CMAKE_LIBRAY_OUTPUT_DIRECTORY "/home/jjjurado/Dev/OpenGL") 
# message("${CMAKE_LIBRAY_OUTPUT_DIRECTORY}")
# option(MY_DEBUG "Defines MY_DEBUG option to debug output context in OpenGL" ON)
add_subdirectory(GettingStarted)
add_subdirectory(Light)
add_subdirectory(ModelLoading)
