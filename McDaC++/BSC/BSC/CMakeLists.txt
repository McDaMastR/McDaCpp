cmake_minimum_required(VERSION 3.19.5)
project(BSClib VERSION 0.0.1)

set(TEST_FILE 
	main.cpp)

set(SRC_FILES 
    src/types/BSCoperator.cpp 
    src/types/BSCfunction.cpp
    src/grid/BSCoperator.cpp
    src/grid/BSCfunction.cpp
    src/math/BSCarithmatic.cpp
    src/math/BSCprobSolve.cpp)

set(HDR_FILES 
    include/BSCtypes.hpp 
    include/BSCpch.hpp 
    include/BSCmath.hpp
    include/BSCgrid.hpp)

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

set(CXX_COMPILE_DEBUG 
    -g 
    -D DEBUG)

set(ADD_LIB
	-Lbin/static
	-lBSCstatic)

add_compile_options(${CXX_STANDARD_FLAGS} ${CXX_COMPILE_WARNINGS} ${CXX_COMPILE_DEBUG})

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

add_compile_options(${ADD_LIB})

add_executable(test ${TEST_FILE})
