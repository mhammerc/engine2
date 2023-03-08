#pragma once

#include "../common.h"

namespace engine {

struct Transform {
    vec3 position = vec3(0.F);
    glm::quat rotation = glm::identity<glm::quat>();
    vec3 scale = vec3(1.F);

    [[nodiscard]] auto matrix() const -> mat4;

    // Where this transform is pointing at.
    // Base direction: (0,0,1). Then rotated by rotation.
    // This function is optimized to not make redundant computations.
    [[nodiscard]] auto direction() -> vec3;

  private:
    glm::quat _previous_rotation;
    vec3 _direction;
};

namespace reflection {
    auto register_transorm() -> void;
}

}  // namespace engine