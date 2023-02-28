#include "transform_component.h"

using namespace engine;

auto TransformComponent::matrix() const -> mat4 {
    mat4 transform = mat4(1.0F);

    {
        transform = glm::translate(transform, position);

        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.F, 0.F, 0.F));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.F, 1.F, 0.F));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.F, 0.F, 1.F));

        transform = glm::scale(transform, scale);
    }

    return transform;
}
