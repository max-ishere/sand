add_executable(sand_main main.cpp)

find_package(box2d REQUIRED)

add_subdirectory(system)
add_subdirectory(core)
add_subdirectory(dependencies)
target_link_libraries(sand_main
  PRIVATE sand_system
  PRIVATE sand_core
  PRIVATE SDL2
  PRIVATE box2d
  PRIVATE imgui_sdl
  PRIVATE imgui)
