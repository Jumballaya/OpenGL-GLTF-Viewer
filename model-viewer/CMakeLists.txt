add_executable(model-viewer main.cpp)
target_link_libraries(model-viewer PRIVATE core)
set_target_properties(model-viewer PROPERTIES CXX_EXTENSIONS OFF)