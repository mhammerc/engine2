#include "../common.h"
#include "../core/input.hpp"
#include "../scene/components/base_component.h"
#include "../scene/components/camera_component.h"
#include "GLFW/glfw3.h"
#include "systems.h"

using namespace engine;

static auto move_position(float delta_time, Input const& input, BaseComponent& base, CameraComponent& camera) -> void {
    const float camera_movement = camera.speed * delta_time;

    if (input.keys[GLFW_KEY_W]) {
        base.transform.position += camera_movement * base.transform.direction();
    }

    if (input.keys[GLFW_KEY_S]) {
        base.transform.position -= camera_movement * base.transform.direction();
    }

    if (input.keys[GLFW_KEY_A]) {
        base.transform.position -=
            glm::normalize(glm::cross(base.transform.direction(), camera.up_axis)) * camera_movement;
    }

    if (input.keys[GLFW_KEY_D]) {
        base.transform.position +=
            glm::normalize(glm::cross(base.transform.direction(), camera.up_axis)) * camera_movement;
    }

    if (input.keys[GLFW_KEY_SPACE]) {
        base.transform.position += camera.up_axis * camera_movement;
    }

    if (input.keys[GLFW_KEY_LEFT_CONTROL]) {
        base.transform.position -= camera.up_axis * camera_movement;
    }
}

static auto
rotate_camera(Input const& input, BaseComponent& base, CameraComponent& camera, bool& first_frame_mouse_captured)
    -> void {
    static vec2 previous_cursor_position(0.F);
    vec2 cursor_position = input.mouse_position();

    if (first_frame_mouse_captured && previous_cursor_position != cursor_position) {
        previous_cursor_position = cursor_position;
        first_frame_mouse_captured = false;
    }

    // angle_to_rotate.x is the yaw
    // angle_to_rotate.y is the pitch
    vec2 angle_to_rotate = (cursor_position - previous_cursor_position) * vec2(-1.F, 1.F);

    angle_to_rotate *= camera.cursor_sensivity;

    camera.yaw += angle_to_rotate.x;
    camera.pitch += angle_to_rotate.y;

    const float maxPitch = 89.F;
    if (camera.pitch > maxPitch) {
        camera.pitch = maxPitch;
    }
    if (camera.pitch < -maxPitch) {
        camera.pitch = -maxPitch;
    }

    camera.update_base_rotation(base);

    previous_cursor_position = cursor_position;
}

auto systems::camera_system(float delta_time, entt::registry& registry) -> void {
    auto const& input = entt::locator<Input>::value();
    auto* window = entt::locator<GLFWwindow*>::value();

    auto view = registry.view<BaseComponent, CameraComponent>();

    for (auto [entity, base, camera] : view.each()) {
        if (!base.enabled) {
            continue;
        }

        static bool mouse_captured = false;
        static bool first_frame_mouse_captured = false;

        if (input.keys[GLFW_KEY_R]) {
            mouse_captured = true;
            first_frame_mouse_captured = true;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        if (input.keys[GLFW_KEY_T]) {
            mouse_captured = false;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (mouse_captured) {
            move_position(delta_time, input, base, camera);

            rotate_camera(input, base, camera, first_frame_mouse_captured);
        }
    }
}