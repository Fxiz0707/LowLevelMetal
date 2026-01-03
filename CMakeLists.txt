cmake_minimum_required(VERSION 3.29)
project(exploring_blas LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJCXX_STANDARD 20)

find_package(BLAS REQUIRED)

add_subdirectory(metal-cmake)

if (BLAS_FOUND)
    message(STATUS "BLAS Libraries: ${BLAS_LIBRARIES}")
endif()

add_executable(exploring_blas main.cpp
        Benchmark.cpp
        Benchmark.h
        App.cpp
        App.h
        MyAppDelegate.cpp
        MyAppDelegate.h
        MyMTKViewDelegate.cpp
        MyMTKViewDelegate.h
        Renderer.mm
        Renderer.h
        MyUtility.cpp
        MyUtility.h)

target_link_libraries(exploring_blas PRIVATE BLAS::BLAS METAL_CPP)

# Copy shaders to build directory
file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/meshes" DESTINATION "${CMAKE_BINARY_DIR}")

