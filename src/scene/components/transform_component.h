#pragma once

#include "../../common.h"

namespace engine {

struct TransformComponent {
    vec3 position = vec3(0.F);
    vec3 rotation = vec3(0.F);
    vec3 scale = vec3(1.F);

    [[nodiscard]] auto matrix() const -> mat4;
};

namespace reflection {
    auto register_transform_component() -> void;
}

}  // namespace engine