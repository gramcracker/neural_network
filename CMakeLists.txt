cmake_minimum_required(VERSION 3.10)

project(neuralnetwork)



file(GLOB_RECURSE libsrc src/*.cpp)
add_library(neuralnetwork SHARED ${libsrc})
target_link_libraries(neuralnetwork pthread)
target_include_directories(neuralnetwork PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#for using python:
#(requires installing python3-pybind11)
find_package(pybind11 REQUIRED)

#use this instead for local pybind11 lib package:
#(copy pybind11 into python folder)
# add_subdirectory(python/pybind11)
# target_include_directories(pyneuralnetwork PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/python/pybind11/include )
#also comment out this line:
pybind11_add_module(pyneuralnetwork python/pyInterface.cpp)

target_link_libraries(pyneuralnetwork PUBLIC neuralnetwork)
set_target_properties(neuralnetwork PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set_target_properties(pyneuralnetwork PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python)

