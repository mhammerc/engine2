#include "transform_component.h"

auto TransformComponent::position() -> vec3& {
    return _position;
}

auto TransformComponent::rotation() -> vec3& {
    return _rotation;
}

auto TransformComponent::scale() -> vec3& {
    return _scale;
}
