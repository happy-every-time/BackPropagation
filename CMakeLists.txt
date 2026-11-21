cmake_minimum_required(VERSION 4.4)
project(BackPropagation)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BackPropagation
        artificial_neural_network_node.cpp
        artificial_neural_network_node.hpp
        base_node.cpp
        base_node.hpp
        main.cpp)
