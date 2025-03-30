#include "Window.hpp"
#include <iostream>

Core::Window::Window(int width, int height, const char *name)
{
  _width = width;
  _height = height;

  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, name, nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return;
  }

  std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
  glViewport(0, 0, width, height);
}

Core::Window::~Window()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Core::Window::swapBuffers()
{
  glfwSwapBuffers(window);
}

void Core::Window::pollEvents()
{
  glfwPollEvents();
}

bool Core::Window::shouldClose() const
{
  return glfwWindowShouldClose(window);
}