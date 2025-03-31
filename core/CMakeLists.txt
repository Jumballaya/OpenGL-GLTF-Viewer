add_library(
  core STATIC
  src/core/Window.cpp
  src/core/Window.hpp
  src/core/GL/GLBuffer.hpp
  src/core/GL/GLShader.hpp
  src/core/GL/GLTexture.cpp
  src/core/GL/GLTexture.hpp
  src/core/GL/VAO.hpp
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

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)
add_library(stb_image INTERFACE)

target_include_directories(core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${glfw_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
  ${stb_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(core PRIVATE
  glfw
  OpenGL::GL
  glm
  stb_image
)

set_target_properties(core PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
)