cmake_minimum_required(VERSION 3.18)

project(SimUDuck)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SimUDuck main.cpp)

FILE(GLOB_RECURSE RESOURCES "src/*.cpp" "src/*.h")
add_library(library ${RESOURCES})

target_link_libraries(SimUDuck PRIVATE library)
