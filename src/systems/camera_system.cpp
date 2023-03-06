#include "../core/input.hpp"
#include "../scene/components/camera_component.h"
#include "../scene/components/name_component.h"
#include "../utils/glfw3.h"
#include "systems.h"

using namespace engine;

auto systems::camera_system(float delta_time, entt::registry& registry) -> void {
    auto const& input = entt::locator<Input>::value();
    auto* window = entt::locator<GLFWwindow*>::value();

    auto view = registry.view<NameComponent, CameraComponent>();

    for (auto [entity, name, camera] : view.each()) {
        if (!name.enabled) {
            continue;
        }

        static bool first_mouse = true;
        static bool mouse_captured = false;

        if (input.keys[GLFW_KEY_R]) {
            first_mouse = true;
            mouse_captured = true;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        if (input.keys[GLFW_KEY_T]) {
            first_mouse = true;
            mouse_captured = false;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (mouse_captured) {
            const float camera_movement = camera.speed * delta_time;

            if (input.keys[GLFW_KEY_W]) {
                camera.position += camera_movement * camera.front_direction;
            }

            if (input.keys[GLFW_KEY_S]) {
                camera.position -= camera_movement * camera.front_direction;
            }

            if (input.keys[GLFW_KEY_A]) {
                camera.position -= glm::normalize(glm::cross(camera.front_direction, camera.up_axis)) * camera_movement;
            }

            if (input.keys[GLFW_KEY_D]) {
                camera.position += glm::normalize(glm::cross(camera.front_direction, camera.up_axis)) * camera_movement;
            }

            if (input.keys[GLFW_KEY_SPACE]) {
                camera.position += camera.up_axis * camera_movement;
            }

            if (input.keys[GLFW_KEY_LEFT_CONTROL]) {
                camera.position -= camera.up_axis * camera_movement;
            }
        }

        if (mouse_captured) {
            static vec2 previous_cursor_position(0);

            if (first_mouse) {
                previous_cursor_position = input.mouse_position();
                first_mouse = false;
            }

            vec2 cursor_delta = input.mouse_position() - previous_cursor_position;

            cursor_delta *= camera.cursor_sensivity;

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
            previous_cursor_position = input.mouse_position();
        }
    }
}