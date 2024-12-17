cmake_minimum_required(VERSION 3.10)
project(majnkraft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

find_package(OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(glm REQUIRED)

include_directories(${SFML_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/ShaderProgram.cpp
    src/PerlinNoise.cpp
    src/CubePalette.cpp
    src/AABB.cpp
    src/Ray.cpp
)

set(HEADERS
    include/Camera.hpp
    include/Cube.hpp
    include/ShaderProgram.hpp
    include/Chunk.hpp
    include/PerlinNoise.hpp
    include/CubePalette.hpp
    include/Chunk.hpp
    include/AABB.hpp
    include/Ray.hpp
)

add_executable(majnkraft ${SOURCES} ${HEADERS})

target_link_libraries(majnkraft glad ${OPENGL_LIBRARIES} ${SFML_LIBRARIES} sfml-graphics)

if (UNIX)
    target_link_libraries(majnkraft dl)
endif()
