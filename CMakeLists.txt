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
        ApplicationState.cpp
        ApplicationState.h
        MyAppDelegate.cpp
        MyAppDelegate.h
        MyMTKViewDelegate.cpp
        MyMTKViewDelegate.h
        Renderer.mm
        Renderer.h
        MyUtility.cpp
        MyUtility.h
        MeshLoader.h
        Mesh.h
        Math.h
        ShaderTypes.h
        RenderDataManager.h
        RenderDataManager.cpp
        InputHandler.h
        InputHandler.cpp
        InputMTKView.h
        InputMTKView.mm
)

target_link_libraries(exploring_blas PRIVATE BLAS::BLAS METAL_CPP)

# Copy shaders and meshes to build directory on EVERY build (always runs)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/meshes ${CMAKE_BINARY_DIR}/meshes
    COMMENT "Copying shaders and meshes to build directory")

# Ensure assets are copied before running the executable
add_dependencies(exploring_blas copy_assets)

