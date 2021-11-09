find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(sand_test main.cpp)

target_compile_options(sand_test
    PUBLIC ${GTEST_CXXFLAGS})

target_link_libraries(sand_test
    PUBLIC ${GTEST_LDFLAGS})

set_target_properties(sand_test PROPERTIES
  CXX_STANDARD 17)

add_custom_command(TARGET sand_test
  POST_BUILD
  COMMAND sand_test
  COMMENT "Running gtest on build")
