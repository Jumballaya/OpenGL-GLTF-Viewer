#include <iostream>
#include <core/Window.hpp>
#include <core/GL/GLBuffer.hpp>
#include <core/GL/VAO.hpp>
#include <core/GL/GLShader.hpp>

int main()
{
  std::vector<float> vertices = {
      -0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f,
      0.5f, -0.5f, 0.0f};
  std::vector<unsigned int> indices = {0, 1, 2};

  Core::Window window(800, 600, "Triangle");

  Core::GL::GLBuffer vbo(Core::GL::BufferType::Vertex);
  Core::GL::GLBuffer ebo(Core::GL::BufferType::Index);
  Core::GL::VAO vao;

  if (auto error = vbo.setData(std::span(vertices)); error)
  {
    std::cerr << "VBO Error: " << *error << std::endl;
    return -1;
  }

  if (auto error = ebo.setData(std::span(indices)); error)
  {
    std::cerr << "EBO Error: " << *error << std::endl;
    return -1;
  }

  vao.addVertexBuffer(vbo, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
  vao.setIndexBuffer(ebo);

  Core::GL::GLShader shader("assets/shaders/basic/vertex.glsl", "assets/shaders/basic/fragment.glsl");

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
                         { std::cerr << "OpenGL Debug Message: " << message << std::endl; }, nullptr);

  while (!window.shouldClose())
  {
    window.pollEvents();

    glClearColor(0.82, 0.0, 0.07, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    vao.bind();
    shader.use();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    vao.unbind();

    window.swapBuffers();
  }

  return 0;
}