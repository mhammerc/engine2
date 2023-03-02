#pragma once

#include "../common.h"
#include "GLFW/glfw3.h"

using namespace engine;

class Camera {
  public:
    [[nodiscard("hey")]] auto getMatrix() const -> glm::mat4;

    vec3 pos = vec3(0.0F, 0.0F, 3.0F);
    vec3 front = vec3(0.0F, 0.0F, -1.0F);
    vec3 up = vec3(0.0F, 1.0F, 0.0F);

    float yaw = -90.F;
    float pitch = 0.F;
    vec2 previous_cursor_position;

    auto computeFront() -> void;

    explicit Camera(GLFWwindow* window);
};