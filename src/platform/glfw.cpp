#include "glfw.h"

#include <entt/entt.hpp>

#include "../core/input.hpp"
#include "../utils/platform_info.h"
#include "GLFW/glfw3.h"
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

static auto key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mode*/) {
    auto& input = entt::locator<engine::Input>::value();

    input.on_key(key, action == GLFW_PRESS || action == GLFW_REPEAT);
}

static auto mouse_button_callback(GLFWwindow* /*window*/, int key, int action, int /*mode*/) {
    auto& input = entt::locator<engine::Input>::value();

    input.on_key(key, action == GLFW_PRESS || action == GLFW_REPEAT);
}

static auto mouse_position_callback(GLFWwindow* /*window*/, double xpos, double ypos) {
    auto& input = entt::locator<engine::Input>::value();
    auto position = vec2(static_cast<float>(xpos), static_cast<float>(ypos));

    input.on_mouse_position(position);
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

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);

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