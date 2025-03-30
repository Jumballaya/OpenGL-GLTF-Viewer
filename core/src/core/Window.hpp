#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core
{
  class Window
  {
  public:
    Window(int width, int height, const char *name);
    ~Window();

    void swapBuffers();
    void pollEvents();
    bool shouldClose() const;

  private:
    int _width, _height;
    GLFWwindow *window;
  };
}
