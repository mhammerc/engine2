#include "base_component.h"

#include "../common.h"
#include "entt/entt.hpp"
#include "glm/gtx/matrix_decompose.hpp"

using namespace engine;

BaseComponent::BaseComponent(std::string_view const& name) : name(name) {}

auto BaseComponent::world_matrix(entt::registry const& registry) const -> mat4 {
    mat4 matrix = transform.matrix();

    if (parent != entt::null) {
        // If we have a parent, our transform "inherit" the parent transform
        auto const& parent_base = registry.get<BaseComponent>(parent);
        auto parent_matrix = parent_base.world_matrix(registry);

        matrix = parent_matrix * matrix;
    }

    return matrix;
}

auto BaseComponent::world_transform(entt::registry const& registry) const -> Transform {
    mat4 matrix = world_matrix(registry);

    Transform world;
    vec3 _skew;
    vec4 _perspective;
    glm::decompose(matrix, world.scale, world.rotation, world.position, _skew, _perspective);

    return world;
}

auto reflection::register_base_component() -> void {
    auto factory = entt::meta<BaseComponent>();
    factory.prop("name"_hs, "BaseComponent");

    factory.data<&BaseComponent::name>("name"_hs).prop("name"_hs, "name");
    factory.data<&BaseComponent::enabled>("enabled"_hs).prop("name"_hs, "enabled");
    factory.data<&BaseComponent::transform>("transform"_hs).prop("name"_hs, "transform");

    factory.func<&BaseComponent::world_transform>("world_transform"_hs).prop("name"_hs, "world_transform");
}