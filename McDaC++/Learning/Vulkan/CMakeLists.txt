cmake_minimum_required(VERSION 3.20.3)
project(LearnVulkan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE_FILES
    src/main.cpp
    src/application.cpp)

set(COMPILE_FLAGS
    -Og
    -Wall 
    -Wextra 
    -Wpedantic 
    -Wshadow
    -Wnon-virtual-dtor 
    -Woverloaded-virtual 
    -Wold-style-cast 
    -Wcast-align 
    -Wunused 
    -Wconversion 
    -Wsign-conversion 
    -Wnull-dereference 
    -Wdouble-promotion 
    -Wformat=2)

set(MACROS
    DEBUG)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3.4 REQUIRED)

add_compile_definitions(${MACROS})
add_compile_options(${COMPILE_FLAGS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_custom_command(
	TARGET ${PROJECT_NAME}
	DEPENDS ${CMAKE_SOURCE_DIR}/shaders/triangle.vert

	COMMENT "[ __%] Compiling Vertex Shader"
	COMMAND glslc ${CMAKE_SOURCE_DIR}/shaders/triangle.vert -o ${CMAKE_SOURCE_DIR}/bin/vert.spv
)

add_custom_command(
	TARGET ${PROJECT_NAME}
	DEPENDS ${CMAKE_SOURCE_DIR}/shaders/triangle.frag

	COMMENT "[ __%] Compiling Fragment Shader"
	COMMAND glslc ${CMAKE_SOURCE_DIR}/shaders/triangle.frag -o ${CMAKE_SOURCE_DIR}/bin/frag.spv
)

target_include_directories(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} glfw)
