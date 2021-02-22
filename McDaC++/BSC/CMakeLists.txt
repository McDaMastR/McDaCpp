cmake_minimum_required(VERSION 3.19.5)
project(BSClib VERSION 0.0.1)

set(SRC_FILES 
    src/types/BSCoperator.cpp 
    src/types/BSCconstruct.cpp)

set(HDR_FILES 
    include/BSCtypes.hpp 
    include/BSCpch.hpp 
    include/BSCmath.hpp 
    include/BSCmultiBool.hpp)

set(TEST_FILE src/main.cpp)

set(CXX_STANDARD_FLAGS 
    -std=gnu++2a)

set(CXX_COMPILE_WARNINGS 
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

set(CXX_COMPILE_SANITIZERS 
    -fsanitize=address 
    -fsanitize=undefined)

set(CXX_COMPILE_DEBUG 
    -g 
    -D DEBUG)

add_library(
    BSCstatic
    STATIC
    ${SRC_FILES}
    ${HDR_FILES}
)

add_library(
    BSCshared
    SHARED
    ${SRC_FILES}
    ${HDR_FILES}
)

add_compile_options(${CXX_STANDARD_FLAGS} ${CXX_COMPILE_WARNINGS} ${CXX_COMPILE_SANITIZERS} ${CXX_COMPILE_DEBUG})
add_executable(test ${SRC_FILES})