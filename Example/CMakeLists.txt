cmake_minimum_required(VERSION 3.10)
project(Example)
set(CMAKE_CXX_STANDARD 14)

add_executable(Example main.cpp)

add_subdirectory(.. kdalgorithmsbuild)
# Would normally be something like
# add_subdirectory(3rdparty/kdalgorithms)
target_link_libraries(Example kdalgorithms)
