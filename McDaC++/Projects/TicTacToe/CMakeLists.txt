cmake_minimum_required(VERSION 3.19.5)
project(TicTacToe)

set(MAIN_FILE
    src/main.cpp)

set(PROJECT_SOURCE_FILES
    src/renderer.cpp
    src/vertexArray.cpp
    src/indexBuffer.cpp
    src/shader.cpp
    src/vertexBuffer.cpp
    src/vertexBufferLayout.cpp
    src/texture.cpp
	src/square.cpp
    src/vertexBufferElement.cpp
	src/blender.cpp
	src/events.cpp
	src/window.cpp
	src/board.cpp
	src/piece.cpp
	src/dearImGui.cpp)

set(VENDOR_SOURCE_FILES
	src/vendor/stb_image/stb_image.cpp
	src/vendor/imGui/imgui_demo.cpp
	src/vendor/imGui/imgui_draw.cpp
	src/vendor/imGui/imgui_impl_glfw.cpp
	src/vendor/imGui/imgui_impl_opengl3.cpp
	src/vendor/imGui/imgui_tables.cpp
	src/vendor/imGui/imgui_widgets.cpp
	src/vendor/imGui/imgui.cpp)

set(DEBUG_COMPILE_FLAGS
    -g 
    -Og)

set(WARNING_COMPILE_FLAGS
    -Wall
    -Wextra
    -Wpedantic
	-Wno-deprecated-volatile)

set(DEBUG_MACROS
    DEBUG)

set(RELEASE_MACROS
	NDEBUG)

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

add_executable(${PROJECT_NAME} ${MAIN_FILE} ${PROJECT_SOURCE_FILES} ${VENDOR_SOURCE_FILES})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)