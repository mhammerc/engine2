#include <imgui/imgui.h>

#include <entt/entt.hpp>

#include "../common.h"
#include "../graphics/framebuffer_cache.h"
#include "ui_internal.h"

using namespace engine;

static auto draw_combo_box(FrameBufferCache const& framebuffer_cache, u32& current_index) -> void {
    auto current = framebuffer_cache[current_index];

    if (ImGui::BeginCombo("Framebuffer", current->first.c_str())) {
        for (auto framebuffer_pair : framebuffer_cache) {
            auto index = framebuffer_pair.first;
            auto const& name = framebuffer_pair.second->first;

            if (ImGui::Selectable(name.c_str(), index == current_index)) {
                current_index = index;
            }
        }

        ImGui::EndCombo();
    }
}

static auto draw_framebuffer(FrameBufferCache const& framebuffer_cache, u32& current_index) -> void {
    auto const& framebuffer = framebuffer_cache[current_index]->second;

    void* framebuffer_texture_handle = reinterpret_cast<void*>(framebuffer.color_texture()->handle());

    auto window_size = ImGui::GetContentRegionAvail();
    vec2 size;
    if (window_size.x < (window_size.y + 70)) {
        float ratio = static_cast<float>(framebuffer.size().y) / static_cast<float>(framebuffer.size().x);

        size = {window_size.x, window_size.x * ratio};
    } else {
        float ratio = static_cast<float>(framebuffer.size().x) / static_cast<float>(framebuffer.size().y);

        size = {window_size.y * ratio, window_size.y};
    }

    float dpi_scale = ImGui::GetWindowDpiScale();
    size *= dpi_scale;

    ImGui::Image(framebuffer_texture_handle, {size.x, size.y}, ImVec2(0, 1), ImVec2(1, 0));
}

auto ui::internal::ui_draw_window_framebuffer_viewer(bool* is_open) -> void {
    if (!*is_open) {
        return;
    }

    auto& framebuffer_cache = entt::locator<FrameBufferCache>::value();
    static u32 current_index = 0;

    // If none is selected, select the first one
    if (current_index == 0) {
        if (auto begin = framebuffer_cache.begin(); begin != framebuffer_cache.end()) {
            current_index = begin->first;
        }
    }

    ImGui::Begin("Framebuffer Viewer", is_open);

    draw_combo_box(framebuffer_cache, current_index);
    draw_framebuffer(framebuffer_cache, current_index);

    ImGui::End();
}