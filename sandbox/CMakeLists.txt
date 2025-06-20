cmake_minimum_required(VERSION 3.12)

project(sandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sandbox main.cpp)

target_link_libraries(sandbox PRIVATE Nova)