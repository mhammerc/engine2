#include "opengl.h"

#include "../utils/glfw3.h"

auto load_opengl() -> bool {
    return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) != 0;
}

auto list_extensions() -> void {
    GLint n = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);

    for (GLint i = 0; i < n; ++i) {
        const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        spdlog::info("Ext {}: {}", i, extension);
    }
}