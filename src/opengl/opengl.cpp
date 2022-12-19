#include "opengl.h"
#include <GLFW/glfw3.h>

bool load_opengl() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	return false;
  }

  return true;
}