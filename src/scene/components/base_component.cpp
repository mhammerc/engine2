#include "base_component.h"

#include <entt/entt.hpp>

#include "../../common.h"

using namespace engine;

BaseComponent::BaseComponent(std::string_view const& name) : name(name) {}

auto BaseComponent::direction() const -> vec3 {
    auto constexpr forward = vec3(0.F, 0.F, 1.F);

    auto static previous_rotation = rotation;
    auto static direction = glm::normalize(rotation * forward);

    if (previous_rotation != rotation) {
        // If BaseComponent::rotation member have changed since last call,
        // compute the new direction.

        previous_rotation = rotation;
        direction = glm::normalize(rotation * forward);
    }

    return direction;
}

auto BaseComponent::transform_matrix(entt::registry const& registry) const -> mat4 {
    mat4 transform = mat4(1.0F);

    {
        transform = glm::translate(transform, position);

        transform = transform * glm::mat4_cast(rotation);

        transform = glm::scale(transform, scale);
    }

    if (parent != entt::null) {
        // If we have a parent, our transform "inherit" the parent transform
        auto const& parent_base = registry.get<BaseComponent>(parent);
        auto parent_transform = parent_base.transform_matrix(registry);

        transform = parent_transform * transform;
    }

    return transform;
}

auto reflection::register_name_component() -> void {
    auto factory = entt::meta<BaseComponent>();
    factory.prop("name"_hs, "BaseComponent");

    factory.data<&BaseComponent::name>("name"_hs).prop("name"_hs, "name");
    factory.data<&BaseComponent::enabled>("enabled"_hs).prop("name"_hs, "enabled");

    factory.data<&BaseComponent::position>("position"_hs).prop("name"_hs, "position");
    factory.data<&BaseComponent::rotation>("rotation"_hs).prop("name"_hs, "rotation");
    factory.data<&BaseComponent::scale>("scale"_hs).prop("name"_hs, "scale");
    factory.func<&BaseComponent::transform_matrix>("transform_matrix"_hs).prop("name"_hs, "transform_matrix");
}