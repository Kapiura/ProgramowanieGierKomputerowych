cmake_minimum_required(VERSION 3.10)
project(task1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

find_package(OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS window REQUIRED)

include_directories(include ${OPENGL_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})

set(SOURCES
    src/main.cpp
    src/Render.cpp
    src/ShaderProgram.cpp
)

set(HEADERS
    include/Render.hpp
    include/ShaderProgram.hpp
)

add_executable(task1 ${SOURCES} ${HEADERS})

target_link_libraries(task1 glad ${OPENGL_LIBRARIES} sfml-window)

if (UNIX)
    target_link_libraries(task1 dl)  # Link dynamic library loader on Linux
endif()
