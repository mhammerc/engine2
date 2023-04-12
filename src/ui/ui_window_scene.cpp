#include <tuple>

#include "../common.h"
#include "../components/outline_component.h"
#include "../core/editor_selected_entity.h"
#include "../utils/opengl_handle_to_pointer.h"
#include "imgui.h"
#include "imgui/ImGuizmo.h"
#include "imgui/imgui.h"
#include "ui_internal.h"

using namespace engine;

// return <image position, image size, is_window_hovered>
static auto draw(entt::registry& registry, Framebuffer* scene_texture, entt::entity selected_entity)
    -> std::tuple<vec2, vec2, bool> {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Scene");

    vec2 image_pos = ImGui::GetCursorScreenPos();

    vec2 image_size = ImGui::GetContentRegionAvail();

    float dpi_scale = ImGui::GetWindowDpiScale();

    scene_texture->resize({image_size.x * dpi_scale, image_size.y * dpi_scale});

    ImGui::Image(
        opengl_handle_to_pointer(scene_texture->attachments()[0].second->handle()),
        {image_size},
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::SetCursorScreenPos(image_pos + vec2 {0, 6});

    auto operation_mode = ui::internal::ui_draw_widget_transform();

    ui::internal::ui_gizmo_manipulate_3d_object(
        registry,
        {
            .position = image_pos,
            .size = image_size,
            .operation = operation_mode.first,
            .mode = operation_mode.second,
        },
        selected_entity
    );

    auto is_hovered = ImGui::IsWindowHovered();

    ImGui::End();

    ImGui::PopStyleVar(1);

    return {image_pos, image_size, is_hovered};
}

auto ui::internal::ui_draw_window_scene(
    entt::registry& registry,
    Framebuffer* scene_texture,
    entt::entity selected_entity
) -> void {
    auto [image_pos, image_size, scene_hovered] = draw(registry, scene_texture, selected_entity);

    vec2 mouse_pos = ImGui::GetMousePos();
    vec2 mouse_over_scene = (mouse_pos - image_pos) / image_size;

    if (mouse_over_scene.x != std::clamp(mouse_over_scene.x, 0.F, 1.F)
        || mouse_over_scene.y != std::clamp(mouse_over_scene.y, 0.F, 1.F)) {
        // Mouse is outside of the scene view
        return;
    }

    // Get the identify texture and identify which object it is
    vec2 pixel_position = {mouse_over_scene.x, 1.F - mouse_over_scene.y};
    pixel_position *= scene_texture->size();

    vec4 pixel_color;
    scene_texture->bind();
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glReadPixels(
        static_cast<int>(pixel_position.x),
        static_cast<int>(pixel_position.y),
        1,
        1,
        GL_RGBA,
        GL_FLOAT,
        &pixel_color
    );
    scene_texture->unbind();

    u32 const entity_id = static_cast<int>(pixel_color.r);

    if (entity_id == 0) {
        return;
    }

    auto entity = static_cast<entt::entity>(entity_id - 1);

    if (!registry.valid(entity)) {
        return;
    }

    bool mouse_is_valid = true;
    static bool previous_mouse_is_valid = true;

    mouse_is_valid &= scene_hovered;
    mouse_is_valid &= !ImGuizmo::IsUsing();
    mouse_is_valid &= !ImGuizmo::IsOver();

    if (mouse_is_valid) {
        registry.get_or_emplace<OutlineComponent>(entity);
    }

    if (mouse_is_valid && previous_mouse_is_valid && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        set_editor_selected_entity(registry, entity);
        return;
    }

    previous_mouse_is_valid = mouse_is_valid;
}