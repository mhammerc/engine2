#include "camera_component.h"

#include <cmath>

#include "entt/entt.hpp"

using namespace engine;

auto CameraComponent::view_matrix(BaseComponent& base) const -> mat4 {
    // TODO: this is local space transform
    auto transform = base.transform;

    return glm::lookAt(transform.position, transform.position + transform.direction(), up_axis);
}

auto CameraComponent::update_base_rotation(BaseComponent& base) const -> void {
    auto rotation_yaw = glm::angleAxis(glm::radians(yaw), vec3(0.F, 1.F, 0.F));
    auto rotation_pitch = glm::angleAxis(glm::radians(pitch), vec3(1.F, 0.F, 0.F));

    base.transform.rotation = rotation_yaw * rotation_pitch;
}

auto reflection::register_camera_component() -> void {
    auto factory = entt::meta<CameraComponent>();
    factory.prop("name"_hs, "CameraComponent");

    factory.data<&CameraComponent::up_axis>("up_axis"_hs).prop("name"_hs, "up_axis");
    factory.data<&CameraComponent::yaw>("yaw"_hs).prop("name"_hs, "yaw");
    factory.data<&CameraComponent::pitch>("pitch"_hs).prop("name"_hs, "pitch");
    factory.data<&CameraComponent::fov>("fov"_hs).prop("name"_hs, "fov");
    factory.data<&CameraComponent::speed>("speed"_hs).prop("name"_hs, "speed");
    factory.data<&CameraComponent::cursor_sensivity>("cursor_sensivity"_hs).prop("name"_hs, "cursor_sensivity");

    factory.func<&CameraComponent::view_matrix>("view_matrix"_hs).prop("name"_hs, "view_matrix");
}
