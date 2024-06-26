#AI output

cmake_minimum_required(VERSION 3.8)
project(projekt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenGL library preference to GLVND
set(OpenGL_GL_PREFERENCE "GLVND")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

# Find the necessary packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Add include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# Globbing to collect all source files in the src directory
file(GLOB SOURCES "src/*.cpp")

#Pozn: not much difference between different levels of optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") 

# Add executable
add_executable(main ${SOURCES})

# Link against the necessary libraries
target_link_libraries(main ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES} glut jsoncpp)



