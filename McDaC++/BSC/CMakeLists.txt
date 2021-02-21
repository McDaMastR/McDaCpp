cmake_minimum_required(VERSION 3.19.5)
project(BSClib VERSION 0.0.1)

add_subdirectory(src)
add_subdirectory(include)

set(SRC_FILES src/types/BSCoperator.cpp src/types/BSCconstruct.cpp)
set(HDR_FILES include/BSCtypes.hpp include/BSCpch.hpp include/BSCmath.hpp include/BSCmultiBool.hpp)
set(TEST_FILE src/main.cpp)

set(COMPILE_FLAGS -Wall -Wextra -Wpedantic -Wshadow)

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

add_compile_options(${COMPILE_FLAGS})
add_executable(test ${SRC_FILES})