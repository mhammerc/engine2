#include <entt/entt.hpp>

#include "../graphics/framebuffer.h"
#include "../utils/glfw3.h"

namespace engine::ui::internal {
auto ui_draw_window_system(float delta_time, GLFWwindow* window) -> void;

auto ui_draw_window_scene(entt::registry& registry, Framebuffer* scene_texture) -> entt::entity;

auto ui_draw_window_hierarchy(entt::registry& registry, entt::entity& currently_selected) -> void;
auto ui_draw_window_entity_editor(entt::registry& registry, entt::entity entity) -> void;

auto ui_draw_window_input_debugger(bool* is_open) -> void;
auto ui_draw_window_texture_viewer(bool* is_open) -> void;
}  // namespace engine::ui::internal