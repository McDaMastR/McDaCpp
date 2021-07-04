cmake_minimum_required(VERSION 3.20.5)
project(LearnVulkan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE_FILES
	src/main.cpp
	src/application.cpp
	src/libraries.cpp)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
	set(PROJECT_NAME "${PROJECT_NAME}-DEBUG")
	
	set(MACROS _DEBUG)
	
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
		-Wformat=2
		-fsanitize=address
		-fsanitize=pointer-compare
		-fsanitize=pointer-subtract
		-fsanitize=undefined
		-Wno-unknown-attributes) # compiler warnings don't recognise likely attributes (but it is supported)
		
	set(LINKER_FLAGS -fsanitize=address)
else()
	set(MACROS)
	set(COMPILE_FLAGS -Wno-unknown-attributes)
	set(LINKER_FLAGS)
endif()

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3.4 REQUIRED)

add_compile_definitions(${MACROS})
add_compile_options(${COMPILE_FLAGS})
add_link_options(${LINKER_FLAGS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_custom_command(
	TARGET ${PROJECT_NAME}
	DEPENDS ${CMAKE_SOURCE_DIR}/assets/shaders/scene.vert

	COMMENT "[100%] Compiling Vertex Shader"
	COMMAND glslc ${CMAKE_SOURCE_DIR}/assets/shaders/scene.vert -o ${CMAKE_SOURCE_DIR}/bin/vert.spv
)

add_custom_command(
	TARGET ${PROJECT_NAME}
	DEPENDS ${CMAKE_SOURCE_DIR}/assets/shaders/scene.frag

	COMMENT "[100%] Compiling Fragment Shader"
	COMMAND glslc ${CMAKE_SOURCE_DIR}/assets/shaders/scene.frag -o ${CMAKE_SOURCE_DIR}/bin/frag.spv
)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} glfw)
