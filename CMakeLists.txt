cmake_minimum_required(VERSION 3.12)

project(PricerOptions CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main
    src/main.cpp
    src/Option.cpp
    src/MathUtils.cpp
)

target_include_directories(main PRIVATE include)