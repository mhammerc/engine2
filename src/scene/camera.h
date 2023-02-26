#pragma once

#include "../platform/opengl.h"

// do not reorganize these imports
#include "GLFW/glfw3.h"

class Camera {
  public:
    [[nodiscard("hey")]] auto getMatrix() const -> glm::mat4;

    glm::vec3 pos = glm::vec3(0.0F, 0.0F, 3.0F);
    glm::vec3 front = glm::vec3(0.0F, 0.0F, -1.0F);
    glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F);

    float yaw = -90.F;
    float pitch = 0.F;
    glm::vec2 previous_cursor_position;

    auto computeFront() -> void;

    explicit Camera(GLFWwindow* window);
};