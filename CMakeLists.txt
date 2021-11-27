cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
project("Sand"
        DESCRIPTION "A game"
        LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions("$<$<CONFIG:DEBUG>:DEBUG>")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_subdirectory(src)
