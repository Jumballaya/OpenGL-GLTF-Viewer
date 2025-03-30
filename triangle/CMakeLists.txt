add_executable(triangle main.cpp)
target_link_libraries(triangle PRIVATE core)
set_target_properties(triangle PROPERTIES CXX_EXTENSIONS OFF)