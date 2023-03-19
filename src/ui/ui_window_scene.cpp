#include <tuple>

#include "../common.h"
#include "../graphics/framebuffer_cache.h"
#include "../scene/components/outline_component.h"
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
        reinterpret_cast<void*>(scene_texture->color_texture()->handle()),
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

    // Get the identify framebuffer and identify which object it is
    auto framebuffer_identify = entt::locator<FramebufferCache>::value()["identify"_hs];

    vec2 pixel_position = {mouse_over_scene.x, 1.F - mouse_over_scene.y};
    pixel_position *= framebuffer_identify->size();

    vec4 pixel_color;
    framebuffer_identify->bind();
    glReadPixels(
        static_cast<int>(pixel_position.x),
        static_cast<int>(pixel_position.y),
        1,
        1,
        GL_RGBA,
        GL_FLOAT,
        &pixel_color
    );
    framebuffer_identify->unbind();

    bool const is_over_object = pixel_color.b <= 0.1F;
    if (!is_over_object) {
        return entt::null;
    }

    u32 const entity_id = static_cast<int>(pixel_color.r * 255.F);
    auto entity = static_cast<entt::entity>(entity_id);

    if (!registry.valid(entity)) {
        return entt::null;
    }

    registry.get_or_emplace<OutlineComponent>(entity);

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        return entity;
    }

    return entt::null;
}