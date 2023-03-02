#include <entt/entt.hpp>

#include "../graphics/framebuffer.h"
#include "../scene/scene.h"

namespace engine::ui::internal {
auto ui_draw_window_system(Scene* scene, float delta_time, GLFWwindow* window) -> void;

auto ui_draw_window_scene(FrameBuffer* scene_texture) -> void;

auto ui_draw_window_hierarchy(entt::registry& registry, entt::entity& currently_selected) -> void;
auto ui_draw_window_entity_editor(entt::registry& registry, entt::entity entity) -> void;
}  // namespace engine::ui::internal