cmake_minimum_required(VERSION 3.10)

project(neuralnetwork)

file(GLOB_RECURSE libsrc src/*.cpp)
add_library(neuralnetwork SHARED ${libsrc})
target_link_libraries(neuralnetwork pthread)
target_include_directories(neuralnetwork PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(neuralnetwork PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# for using python:
    add_subdirectory(python/extern/pybind11)
    pybind11_add_module(pyneuralnetwork python/pyInterface.cpp)
    target_link_libraries(pyneuralnetwork PUBLIC neuralnetwork)

    set_target_properties(pyneuralnetwork PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python)

