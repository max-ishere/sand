cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project("Sand"
        DESCRIPTION "A game"
        # HOMEPAGE_URL
        LANGUAGES CXX
)

# add_subdirectory(src)
add_subdirectory(tests)
include_directories(include)
