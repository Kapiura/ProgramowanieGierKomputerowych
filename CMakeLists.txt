cmake_minimum_required(VERSION 3.10)
project(majnkraft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set policy for OpenGL to prefer GLVND
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

find_package(OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/CubePalette.cpp
    src/ShaderProgram.cpp
    src/PerlinNoise.cpp
)

set(HEADERS
    include/Camera.hpp
    include/Cube.hpp
    include/Chunk.hpp
    include/CubePalette.hpp
    include/ShaderProgram.hpp
    include/PerlinNoise.hpp
)

add_executable(majnkraft ${SOURCES} ${HEADERS})

target_link_libraries(majnkraft glad ${OPENGL_LIBRARIES} ${SFML_LIBRARIES} sfml-graphics)

if (UNIX)
    target_link_libraries(majnkraft dl)
endif()
