cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project("Sand"
        DESCRIPTION "A game"
        # HOMEPAGE_URL
        LANGUAGES CXX
)

# add_subdirectory(src)
include_directories(include)
add_executable(main main.cpp)
target_link_libraries(main
        PUBLIC SDL2)
add_subdirectory(tests)
