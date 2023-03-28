#include <entt/entt.hpp>

#include "../../graphics/deferred_renderer_cache.h"
#include "../imgui.h"
#include "imgui/imgui.h"
#include "ui_window_texture_viewer.h"

using namespace engine;

static auto draw_renderer_list(u32& current_index) -> void {
    auto& deferred_renderer_cache = entt::locator<DeferredRendererCache>::value();

    auto current = deferred_renderer_cache[current_index];
    auto const preview_name = current ? current->name() : "Please select";

    if (ImGui::BeginCombo("Deferred Renderer", preview_name.c_str())) {
        for (auto deferred_renderer_pair : deferred_renderer_cache) {
            auto [index, deferred_renderer] = deferred_renderer_pair;

            auto selectable_name = fmt::format("{}##{}", deferred_renderer->name(), index);

            if (ImGui::Selectable(selectable_name.c_str(), index == current_index)) {
                current_index = index;
            }
        }

        ImGui::EndCombo();
    }
}

static auto draw_texture_list(u32& texture_index) -> void {
    if (ImGui::BeginCombo("Texture", "select")) {
        if (ImGui::Selectable("Albedo", texture_index == 2)) {
            texture_index = 0;
        }

        if (ImGui::Selectable("World Normal", texture_index == 1)) {
            texture_index = 1;
        }

        if (ImGui::Selectable("Specular", texture_index == 3)) {
            texture_index = 2;
        }

        if (ImGui::Selectable("Depth", texture_index == 4)) {
            texture_index = 3;
        }

        ImGui::EndCombo();
    }
}

auto ui::internal::draw_deferred_renderer_viewer() -> void {
    auto& deferred_renderer_cache = entt::locator<DeferredRendererCache>::value();
    static u32 current_index = static_cast<u32>(-1);

    draw_renderer_list(current_index);

    auto renderer = deferred_renderer_cache[current_index];
    if (!renderer) {
        return;
    }

    static u32 texture_index = 0;
    draw_texture_list(texture_index);

    auto* texture = renderer->framebuffer()->attachments()[texture_index].second.get();

    void* texture_handle = reinterpret_cast<void*>(texture->handle());

    vec2 texture_size = texture->size();
    auto window_size = ImGui::GetContentRegionAvail();
    vec2 display_size;
    if (window_size.x < (window_size.y + 70)) {
        float ratio = texture_size.y / texture_size.x;

        display_size = {window_size.x, window_size.x * ratio};
    } else {
        float ratio = texture_size.x / texture_size.y;

        display_size = {window_size.y * ratio, window_size.y};
    }

    ImGui::Image(texture_handle, display_size, ImVec2(0, 1), ImVec2(1, 0));
}