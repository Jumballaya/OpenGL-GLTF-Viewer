#include <iostream>
#include <core/Window.hpp>
#include <GLFW/glfw3.h>

int main()
{
  Core::Window window(800, 600, "Triangle");

  while (!window.shouldClose())
  {
    window.pollEvents();

    glClearColor(0.82, 0.0, 0.07, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    window.swapBuffers();
  }

  return 0;
}