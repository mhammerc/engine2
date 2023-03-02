#include "transform_component.h"

#include <entt/entt.hpp>

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

auto reflection::register_transform_component() -> void {
    auto factory = entt::meta<TransformComponent>();
    factory.prop("name"_hs, "TransformComponent");

    factory.data<&TransformComponent::position>("position"_hs).prop("name"_hs, "position");
    factory.data<&TransformComponent::rotation>("rotation"_hs).prop("name"_hs, "rotation");
    factory.data<&TransformComponent::scale>("scale"_hs).prop("name"_hs, "scale");
    factory.func<&TransformComponent::matrix>("matrix"_hs).prop("name"_hs, "matrix");
}
