#include "camera_component.h"

#include <cmath>
#include <entt/entt.hpp>

using namespace engine;

auto CameraComponent::update_front_direction() -> vec3 {
    vec3 direction;
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

    front_direction = glm::normalize(direction);

    return front_direction;
}

auto CameraComponent::view_matrix() const -> mat4 {
    return glm::lookAt(position, position + front_direction, up_axis);
}

auto reflection::register_camera_component() -> void {
    auto factory = entt::meta<CameraComponent>();
    factory.prop("name"_hs, "CameraComponent");

    factory.data<&CameraComponent::position>("position"_hs).prop("name"_hs, "position");
    factory.data<&CameraComponent::front_direction>("front_direction"_hs).prop("name"_hs, "front_direction");
    factory.data<&CameraComponent::up_axis>("up_axis"_hs).prop("name"_hs, "up_axis");
    factory.data<&CameraComponent::yaw>("yaw"_hs).prop("name"_hs, "yaw");
    factory.data<&CameraComponent::pitch>("pitch"_hs).prop("name"_hs, "pitch");
    factory.func<&CameraComponent::update_front_direction>("update_front_direction"_hs)
        .prop("name"_hs, "update_front_direction");
    factory.func<&CameraComponent::view_matrix>("view_matrix"_hs).prop("name"_hs, "view_matrix");
}