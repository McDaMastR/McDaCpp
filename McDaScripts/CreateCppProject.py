import os

cppCode = R"""#include <iostream>

int main()
{
	std::cout << "Hello, World!\n";
}
"""

cmakeCode = R"""# Cmake minimum version
cmake_minimum_required(VERSION 3.20.3)

# Project Name
project(ProjectName LANGUAGES CXX)

# Set build type to Debug
set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

# Set c++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Set file output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Source files
set(SOURCE_FILES
    src/main.cpp)

# Debug compile flags
set(COMPILE_FLAGS
	-g
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

# Debug macros
set(MACROS
    DEBUG)

# Configure compilation
add_compile_definitions(${MACROS})
add_compile_options(${COMPILE_FLAGS})

# Create the excutable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
"""

def createCmakeFiles():
	with open("CMakeLists.txt", "w") as f:
		f.write(cmakeCode)
	
	os.chdir("build")
	os.system("cmake ..")
	os.system("make")
	os.chdir("..")


def createCppFile():
	os.chdir("src")
	with open("main.cpp", "w") as f:
		f.write(cppCode)
	os.chdir("..")


def createDirectories():
	os.makedirs("bin")
	os.makedirs("src")
	os.makedirs("lib")
	os.makedirs("build")


def main():
	createDirectories()
	createCppFile()
	createCmakeFiles()


if __name__ == "__main__":
	main()