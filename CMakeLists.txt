cmake_minimum_required(VERSION 3.8)
project(projekt)

# Find the necessary packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Add include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") 

# Add executable
set(SOURCES 
    src/main.cpp 
    src/keys.cpp 
    src/window.cpp 
    src/shapes.cpp 
    src/shape_types.cpp 
    src/vec2D.cpp
    src/intersecting.cpp
    src/drawing.cpp
    src/collisions.cpp
)

# Add executable
add_executable(main ${SOURCES})

# Link against the necessary libraries
target_link_libraries(main ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES})


