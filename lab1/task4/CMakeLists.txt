cmake_minimum_required(VERSION 3.18)

project(Drawer)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Drawer main.cpp)

FILE(GLOB_RECURSE RESOURCES "libs/*.cpp" "libs/*.h")
add_library(library ${RESOURCES})

target_link_libraries(Drawer PRIVATE library)
