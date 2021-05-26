cmake_minimum_required(VERSION 3.19)
project(task_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(task_1
        IGraph.cpp
        ListGraph.cpp
        ListGraph.hpp
        MatrixGraph.cpp
        SetGraph.cpp
        ArcGraph.cpp
        main.cpp)
