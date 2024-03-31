cmake_minimum_required(VERSION 3.8)
project(projekt)

# Find the necessary packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Add include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# Add executable
add_executable(main main.cpp keys.cpp window.cpp objects.cpp)

# Link against the necessary libraries
target_link_libraries(main ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES})


