#include "opengl.h"

#include "../utils/glfw3.h"

auto load_opengl() -> bool {
    return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) != 0;
}