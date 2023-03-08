#include "transform.h"

#include <entt/entt.hpp>

using namespace engine;

auto Transform::matrix() const -> mat4 {
    mat4 transform = mat4(1.0F);

    transform = glm::translate(transform, position);

    transform = transform * glm::mat4_cast(rotation);

    transform = glm::scale(transform, scale);

    return transform;
}

auto Transform::direction() -> vec3 {
    auto constexpr forward = vec3(0.F, 0.F, 1.F);

    if (_previous_rotation != rotation) {
        // If BaseComponent::rotation member have changed since last call,
        // compute the new direction.

        _previous_rotation = rotation;
        _direction = glm::normalize(rotation * forward);
    }

    return _direction;
}

auto reflection::register_transorm() -> void {
    auto factory = entt::meta<Transform>();
    factory.prop("name"_hs, "Transform");

    factory.data<&Transform::position>("position"_hs).prop("name"_hs, "position");
    factory.data<&Transform::rotation>("rotation"_hs).prop("name"_hs, "rotation");
    factory.data<&Transform::scale>("scale"_hs).prop("name"_hs, "scale");

    factory.func<&Transform::matrix>("matrix"_hs).prop("name"_hs, "matrix");
    factory.func<&Transform::direction>("direction"_hs).prop("name"_hs, "direction");
}