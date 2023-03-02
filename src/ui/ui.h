#pragma once

#include <entt/entt.hpp>

#include "../graphics/framebuffer.h"
#include "../scene/scene.h"

namespace engine {

auto ui_init(GLFWwindow* window) -> bool;
auto ui_prepare_frame() -> void;
auto ui_draw(float delta_time, Scene* scene, GLFWwindow* window, entt::registry& registry, FrameBuffer* scene_texture)
    -> void;
auto ui_end_frame() -> void;

}  // namespace engine