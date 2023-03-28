#pragma once

#include <entt/entt.hpp>
#include <string>
#include <string_view>

#include "../../core/transform.h"

namespace engine {

struct BaseComponent {
    explicit BaseComponent(std::string_view const& name);
    BaseComponent() = default;

    bool enabled = true;
    std::string name;

    // Relationships
    entt::entity parent = entt::null;
    entt::entity first_child = entt::null;
    entt::entity previous_sibling = entt::null;
    entt::entity next_sibling = entt::null;

    Transform transform;

    [[nodiscard]] auto world_matrix(entt::registry const& registry) const -> mat4;
    [[nodiscard]] auto world_transform(entt::registry const& registry) const -> Transform;
};

namespace reflection {
    auto register_base_component() -> void;
}

};  // namespace engine