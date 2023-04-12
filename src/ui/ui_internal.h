#include <entt/entt.hpp>
#include <tuple>

#include "../graphics/framebuffer.h"
#include "../utils/glfw3.h"
#include "imgui.h"
#include "imgui/ImGuizmo.h"

namespace engine::ui::internal {

auto ui_draw_window_system(float delta_time, GLFWwindow* window) -> void;

auto ui_draw_window_scene(entt::registry& registry, Framebuffer* scene_texture, entt::entity selected_entity) -> void;

auto ui_draw_window_hierarchy(entt::registry& registry, entt::entity currently_selected) -> void;
auto ui_draw_window_entity_editor(entt::registry& registry, entt::entity entity) -> void;

auto ui_draw_window_input_debugger(bool* is_open) -> void;
auto ui_draw_window_texture_viewer(bool* is_open) -> void;
auto ui_draw_window_profiler(bool* is_open) -> void;

/**
 * The ImGui draw cursor must be correctly positionned before calling this function.
 * (ex: top left) 
 */
auto ui_draw_widget_transform() -> std::pair<ImGuizmo::OPERATION, ImGuizmo::MODE>;

struct GizmoManipulate {
    // Position of the rendered image
    vec2 position;

    // Size of the rendered image
    vec2 size;

    ImGuizmo::OPERATION operation;
    ImGuizmo::MODE mode;
};

/**
 * This function draw onto the rendered image the 3D gizmos (translations, scale, rotation).
 *
 * This function must be called inside the window which will render the scene (after Begin() but before End()).
 */
auto ui_gizmo_manipulate_3d_object(entt::registry& registry, GizmoManipulate config, entt::entity selected_entity)
    -> void;

}  // namespace engine::ui::internal