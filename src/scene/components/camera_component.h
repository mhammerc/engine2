#pragma once

#include "../../common.h"

namespace engine {

struct CameraComponent {
    auto update_front_direction() -> vec3;
    [[nodiscard]] auto view_matrix() const -> mat4;

    vec3 position = vec3(0.F, 0.F, 3.F);
    vec3 front_direction = vec3(0.F, 0.F, -1.F);
    vec3 up_axis = vec3(0.F, 1.F, 0.F);

    float yaw = -90.F;
    float pitch = 0.F;
};

namespace reflection {
    auto register_camera_component() -> void;
}

}  // namespace engine