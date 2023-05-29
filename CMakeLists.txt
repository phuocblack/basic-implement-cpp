cmake_minimum_required(VERSION 3.5)

project(obs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(source/*.h)
file(GLOB_RECURSE LIBRARY_HEADERS
    source/*.h)

add_executable(obs main.cpp ${LIBRARY_HEADERS})
