add_subdirectory(system)
add_subdirectory(sdl/system)
add_executable(sand_game main.cpp)
target_link_libraries(sand_game
  PUBLIC sand_system
  PUBLIC sand_sdl_system
  PUBLIC SDL2
  PUBLIC SDL2_image)
