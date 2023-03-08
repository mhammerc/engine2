#pragma once

#include <entt/entt.hpp>
#include <string>
#include <string_view>

#include "../../common.h"

namespace engine {

struct BaseComponent {
    explicit BaseComponent(std::string_view const& name);

    bool enabled = true;
    std::string name;

    // Relationships
    entt::entity parent = entt::null;
    entt::entity first_child = entt::null;
    entt::entity previous_sibling = entt::null;
    entt::entity next_sibling = entt::null;

    // Transform
    vec3 position = vec3(0.F);
    glm::quat rotation = glm::identity<glm::quat>();
    vec3 scale = vec3(1.F);

    // Where this transform is pointing at.
    // Base direction: (0,0,1). Then rotated by rotation.
    // This function is optimized to not make redundant computations.
    [[nodiscard]] auto direction() const -> vec3;

    [[nodiscard]] auto transform_matrix(entt::registry const& registry) const -> mat4;
};

namespace reflection {
    auto register_name_component() -> void;
}

};  // namespace engine