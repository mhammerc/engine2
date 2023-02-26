#include "transform_component.h"

using namespace engine;

auto TransformComponent::matrix() -> mat4 {
    mat4 transform = mat4(1.0F);

    {
        transform = glm::translate(transform, _position);

        transform = glm::rotate(transform, glm::radians(_rotation.x), glm::vec3(1.F, 0.F, 0.F));
        transform = glm::rotate(transform, glm::radians(_rotation.y), glm::vec3(0.F, 1.F, 0.F));
        transform = glm::rotate(transform, glm::radians(_rotation.z), glm::vec3(0.F, 0.F, 1.F));

        transform = glm::scale(transform, _scale);
    }

    return transform;
}

auto TransformComponent::position() -> vec3& {
    return _position;
}

auto TransformComponent::rotation() -> vec3& {
    return _rotation;
}

auto TransformComponent::scale() -> vec3& {
    return _scale;
}
