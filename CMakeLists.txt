cmake_minimum_required(VERSION 3.20)
project(cpp_starter VERSION 0.1.0 LANGUAGES CXX)

# Standard C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json pour l'autocomplétion / clangd / IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(app
    src/main.cpp
)

add_executable(option 
    src/option.cpp
)

target_include_directories(app PRIVATE include)

if(MSVC)
    target_compile_options(app PRIVATE /W4)
else()
    target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
endif()
