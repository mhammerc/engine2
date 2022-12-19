#include "opengl.h"
#include <GLFW/glfw3.h>

auto load_opengl() -> bool {
  return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0;
}