cmake_minimum_required(VERSION 3.19.5)
project(MazeGenerator)

set(MAIN_FILE
    src/main.cpp)

set(SOURCE_FILES
    )

set(HEADER_FILES
    )

set(STANDARD_CXX_COMPILE_FLAGS
    -std=gnu++2a)

set(DEBUG_COMPILE_FLAGS
    -g 
    -Og 
    -D DEBUG)

set(WARNING_COMPILE_FLAGS
    -Wall
    -Wextra
    -Wpedantic)

set(OPENGL_MARROS 
    GL_SILENCE_DEPRECATION 
    GLEW_STATIC)

set(OPENGL_LINKER_FLAGS 
    -lglfw.3.3 
    -lglew.2.2.0 
    -framework OpenGL)

add_compile_definitions(${OPENGL_MARROS})

add_compile_options(${STANDARD_CXX_COMPILE_FLAGS} ${DEBUG_COMPILE_FLAGS} ${WARNING_COMPILE_FLAGS} ${OPENGL_LINKER_FLAGS})

add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SOURCE_FILES})