#include <tuple>

#include "../common.h"
#include "../components/outline_component.h"
#include "../utils/opengl_handle_to_pointer.h"
#include "imgui.h"
#include "imgui/imgui.h"
#include "ui_internal.h"

using namespace engine;

// return <image position, image size>
static auto draw(Framebuffer* scene_texture) -> std::tuple<vec2, vec2> {
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

    ImGui::End();

    ImGui::PopStyleVar(1);

    return {image_pos, image_size};
}

auto ui::internal::ui_draw_window_scene(entt::registry& registry, Framebuffer* scene_texture) -> entt::entity {
    auto [image_pos, image_size] = draw(scene_texture);

    vec2 mouse_pos = ImGui::GetMousePos();
    vec2 mouse_over_scene = (mouse_pos - image_pos) / image_size;

    if (mouse_over_scene.x != std::clamp(mouse_over_scene.x, 0.F, 1.F)
        || mouse_over_scene.y != std::clamp(mouse_over_scene.y, 0.F, 1.F)) {
        // Mouse is outside of the scene view
        return entt::null;
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
        return entt::null;
    }

    auto entity = static_cast<entt::entity>(entity_id - 1);

    if (!registry.valid(entity)) {
        return entt::null;
    }

    registry.get_or_emplace<OutlineComponent>(entity);

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        return entity;
    }

    return entt::null;
}