add_library(
  core STATIC
  src/core/Window.cpp
  src/core/Window.hpp
  src/glad/glad.c
  src/glad/glad.h
  src/KHR/khrplatform.h
)

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

target_include_directories(core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${glfw_SOURCE_DIR}/include
  ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(core PRIVATE
  glfw
  OpenGL::GL
)

set_target_properties(core PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
)