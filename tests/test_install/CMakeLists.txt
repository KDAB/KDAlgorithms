# Tests that users can use find_package and "link" against KDAlgorithms target
# This test is built by the GitHub CI workflow

cmake_minimum_required(VERSION 3.10)

project(test_find_package LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(KDAlgorithms REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE KDAB::kdalgorithms)
