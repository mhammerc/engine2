#include "camera.h"

auto Camera::getMatrix() const -> glm::mat4 {
    return glm::lookAt(pos, pos + front, up);
}

auto Camera::computeFront() -> void {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
}

Camera::Camera(GLFWwindow* window) : previous_cursor_position(0, 0) {
    int width = 0;
    int height = 0;

    glfwGetWindowSize(window, &width, &height);

    previous_cursor_position = glm::vec2(width / 2, height / 2);
}