cmake_minimum_required(VERSION 3.18)

project(SubscriptionTest)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2)

FILE(GLOB_RECURSE RESOURCES "../../src/*.cpp" "../../src/*.h")
add_library(library ${RESOURCES})
target_include_directories(library PUBLIC "../../src")

add_executable(SubscriptionTest subscription.cpp)

target_link_libraries(SubscriptionTest PRIVATE
        library
        Catch2::Catch2WithMain
)