#include <entt/entt.hpp>
#include <optional>

#include "../../common.h"
#include "../../graphics/framebuffer_cache.h"
#include "../imgui.h"
#include "../ui_internal.h"
#include "imgui/imgui.h"
#include "ui_window_texture_viewer.h"

using namespace engine;

auto ui::internal::framebuffer() -> void {
    auto& framebuffer_cache = entt::locator<FrameBufferCache>::value();
    static u32 current_index = static_cast<u32>(-1);

    {
        auto current = framebuffer_cache.contains(current_index) ? std::make_optional(framebuffer_cache[current_index])
                                                                 : std::nullopt;
        auto const* name = current.has_value() ? (*current)->first.c_str() : "Please select";

        if (ImGui::BeginCombo("Framebuffer", name)) {
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

    enum ColorOrDepth { Color, Depth };
    static ColorOrDepth mode = Color;

    {
        char const* mode_text = mode == Color ? "Color" : "Depth/Stencil";

        if (ImGui::BeginCombo("Mode", mode_text)) {
            if (ImGui::Selectable("Color", mode == Color)) {
                mode = Color;
            }

            if (ImGui::Selectable("Depth/Stencil", mode == Depth)) {
                mode = Depth;
            }

            ImGui::EndCombo();
        }
    }

    if (current_index == static_cast<u32>(-1)) {
        return;
    }

    auto const& framebuffer = framebuffer_cache[current_index]->second;

    Texture* texture = nullptr;
    if (mode == Color) {
        texture = framebuffer.color_texture();
    } else if (mode == Depth) {
        texture = framebuffer.depth_stencil_texture();
    }

    if (texture == nullptr) {
        return;
    }

    void* framebuffer_texture_handle = reinterpret_cast<void*>(texture->handle());

    vec2 texture_size = framebuffer.size();
    auto window_size = ImGui::GetContentRegionAvail();
    vec2 size;
    if (window_size.x < (window_size.y + 70)) {
        float ratio = texture_size.y / texture_size.x;

        size = {window_size.x, window_size.x * ratio};
    } else {
        float ratio = texture_size.x / texture_size.y;

        size = {window_size.y * ratio, window_size.y};
    }

    ImGui::Image(framebuffer_texture_handle, size, ImVec2(0, 1), ImVec2(1, 0));
}