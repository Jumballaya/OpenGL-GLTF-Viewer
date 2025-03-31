#include <iostream>
#include <core/Window.hpp>
#include <core/GL/GLBuffer.hpp>
#include <core/GL/VAO.hpp>
#include <core/GL/GLShader.hpp>

int main()
{
  std::vector<float> positions = {
      -1.0f, -1.0f, 0.0f,
      -1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      1.0f, -1.0f, 0.0f};
  std::vector<float> texCoords = {
      0.0f,
      0.0f,
      0.0f,
      1.0f,
      1.0f,
      1.0f,
      1.0f,
      0.0f,
  };
  std::vector<unsigned int> indices = {0, 3, 2, 0, 2, 1};

  Core::Window window(800, 600, "Triangle");

  Core::GL::GLBuffer vboPos(Core::GL::BufferType::Vertex);
  Core::GL::GLBuffer vboUvs(Core::GL::BufferType::Vertex);
  Core::GL::GLBuffer ebo(Core::GL::BufferType::Index);
  Core::GL::VAO vao;

  if (auto error = vboPos.setData(std::span(positions)); error)
  {
    std::cerr << "Positions VBO Error: " << *error << std::endl;
    return -1;
  }

  if (auto error = vboUvs.setData(std::span(texCoords)); error)
  {
    std::cerr << "Tex Coords VBO Error: " << *error << std::endl;
    return -1;
  }

  if (auto error = ebo.setData(std::span(indices)); error)
  {
    std::cerr << "EBO Error: " << *error << std::endl;
    return -1;
  }

  vao.addVertexBuffer(vboPos, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
  vao.addVertexBuffer(vboUvs, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
  vao.setIndexBuffer(ebo);

  Core::GL::GLShader shader("assets/shaders/basic/vertex.glsl", "assets/shaders/basic/fragment.glsl");

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
                         { std::cerr << "OpenGL Debug Message: " << message << std::endl; }, nullptr);

  Core::GL::GLTexture texture;
  texture.loadFromFile("assets/textures/tree.jpg");

  while (!window.shouldClose())
  {
    window.pollEvents();

    glClearColor(0.82, 0.0, 0.07, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    vao.bind();
    shader.use();
    shader.setTexture("u_texture", texture, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    vao.unbind();

    window.swapBuffers();
  }

  return 0;
}