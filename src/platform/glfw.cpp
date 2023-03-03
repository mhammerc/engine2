#include "glfw.h"

#include "../utils/platform_info.h"
#include "opengl.h"
#include "spdlog/spdlog.h"

using namespace engine;

static auto error_callback(int error, const char* description) {
    spdlog::error("GLFW3 Error ({}): {}", error, description);
}

static auto framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

auto process_inputs(float delta_time, GLFWwindow* window, engine::CameraComponent& camera) -> void {
    // KEYBOARD
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    static bool first_mouse = true;
    static bool mouse_captured = false;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        first_mouse = true;
        mouse_captured = true;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        first_mouse = true;
        mouse_captured = false;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (mouse_captured) {
        const float cameraSpeed = 7.F * delta_time;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.position += cameraSpeed * camera.front_direction;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.position -= cameraSpeed * camera.front_direction;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.position -= glm::normalize(glm::cross(camera.front_direction, camera.up_axis)) * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.position += glm::normalize(glm::cross(camera.front_direction, camera.up_axis)) * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.position += camera.up_axis * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera.position -= camera.up_axis * cameraSpeed;
        }
    }

    if (mouse_captured) {
        static glm::vec<2, double> previous_cursor_position(0);
        glm::vec<2, double> cursor_position(0);

        glfwGetCursorPos(window, &cursor_position.x, &cursor_position.y);

        if (first_mouse) {
            previous_cursor_position = cursor_position;
            first_mouse = false;
        }

        glm::vec2 cursor_delta = cursor_position - previous_cursor_position;

        float const sensitivity = 0.1F;
        cursor_delta *= sensitivity;

        camera.yaw += cursor_delta.x;
        camera.pitch += -cursor_delta.y;

        auto const maxPitch = 89.F;
        if (camera.pitch > maxPitch) {
            camera.pitch = maxPitch;
        }
        if (camera.pitch < -maxPitch) {
            camera.pitch = -maxPitch;
        }

        camera.update_front_direction();
        previous_cursor_position = cursor_position;
    }
}

auto init_glfw_and_opengl() -> GLFWwindow* {
    glfwSetErrorCallback(error_callback);

    if (glfwInit() == 0) {
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "OpenGL Engine", nullptr, nullptr);
    if (window == nullptr) {
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!load_opengl()) {
        spdlog::critical("Could not load OpenGL.");
        return nullptr;
    }

    spdlog::info(get_platform_info());

    int framebuffer_width = 0;
    int framebuffer_height = 0;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

    glViewport(0, 0, framebuffer_width, framebuffer_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glEnable(GL_MULTISAMPLE);

    // V-Sync
    // 0 = unlimited FPS on my Mac
    glfwSwapInterval(1);

    return window;
}

auto terminate_glfw(GLFWwindow* window) -> void {
    glfwDestroyWindow(window);
    window = nullptr;

    glfwTerminate();
}