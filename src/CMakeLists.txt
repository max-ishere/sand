add_executable(sand_main main.cpp)

find_package(box2d REQUIRED)

add_subdirectory(system)
target_link_libraries(sand_main
  PUBLIC sand_system
  PUBLIC SDL2
  PUBLIC box2d)
