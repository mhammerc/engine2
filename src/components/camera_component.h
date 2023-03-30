#pragma once

#include "../common.h"
#include "base_component.h"
#include "entt/entt.hpp"

namespace engine {

struct CameraComponent {
    [[nodiscard]] auto view_matrix(BaseComponent& base) const -> mat4;

    vec3 up_axis = vec3(0.F, 1.F, 0.F);

    float yaw = -90.F;
    float pitch = 0.F;

    float fov = 45.F;

    float speed = 7.F;
    float cursor_sensivity = 0.1F;

    float zNear = 0.1F;
    float zFar = 100.F;

    auto update_base_rotation(BaseComponent& base) const -> void;
};

namespace reflection {
    auto register_camera_component() -> void;
}

}  // namespace engine