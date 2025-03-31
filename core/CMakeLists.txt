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

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

target_include_directories(core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${glfw_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(core PRIVATE
  glfw
  OpenGL::GL
  glm
)

set_target_properties(core PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
)