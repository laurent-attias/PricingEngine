cmake_minimum_required(VERSION 3.12)

project(PricerOptions CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main
    src/main.cpp
    src/Option.cpp
    src/MathUtils.cpp
    src/MonteCarloPricer.cpp
)

target_include_directories(main PRIVATE pricer/include)