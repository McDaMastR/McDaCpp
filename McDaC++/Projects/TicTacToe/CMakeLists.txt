cmake_minimum_required(VERSION 3.19.5)
project(MazeGenerator)

set(MAIN_FILE
    src/main.cpp)

set(SOURCE_FILES
    src/maze.cpp
    src/renderer.cpp
    src/vertexArray.cpp
    src/indexBuffer.cpp
    src/shader.cpp
    src/vertexBuffer.cpp
    src/vertexBufferLayout.cpp
    src/texture.cpp
    src/vertexBufferElement.cpp)

set(DEBUG_COMPILE_FLAGS
    -g 
    -Og)

set(WARNING_COMPILE_FLAGS
    -Wall
    -Wextra
    -Wpedantic)

set(OPENGL_LINKER_FLAGS 
    -lglfw.3.3 
    -lglew.2.2.0 
    -framework OpenGL)

set(DEBUG_MACROS
    DEBUG)

set(OPENGL_MARROS 
    GL_SILENCE_DEPRECATION 
    GLEW_STATIC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

add_compile_definitions(${DEBUG_MACROS} ${OPENGL_MARROS})

add_compile_options(${DEBUG_COMPILE_FLAGS} ${WARNING_COMPILE_FLAGS})

add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)