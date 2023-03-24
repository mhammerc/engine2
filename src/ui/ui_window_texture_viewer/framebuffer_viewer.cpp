#include <entt/entt.hpp>
#include <optional>

#include "../../common.h"
#include "../../graphics/framebuffer_cache.h"
#include "../imgui.h"
#include "../ui_internal.h"
#include "ui_window_texture_viewer.h"

using namespace engine;
using namespace ui::internal;

enum ColorOrDepth { Color, Depth };

static auto draw_selector(u32& current_index, FramebufferCache& framebuffer_cache) -> void {
    auto current = framebuffer_cache[current_index];

    auto const* selected_name = current ? current->name().c_str() : "Please select";

    if (ImGui::BeginCombo("Framebuffer", selected_name)) {
        for (auto framebuffer_pair : framebuffer_cache) {
            auto [index, framebuffer] = framebuffer_pair;

            auto selectable_name = fmt::format("{}##{}", framebuffer->name(), index);

            if (ImGui::Selectable(selectable_name.c_str(), index == current_index)) {
                current_index = index;
            }
        }

        ImGui::EndCombo();
    }
}

static auto draw_color_or_depth() -> ColorOrDepth {
    static ColorOrDepth mode = Color;

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

    return mode;
}

static auto draw_texture_2d(Framebuffer* framebuffer, Texture* texture) -> void {
    void* framebuffer_texture_handle = reinterpret_cast<void*>(texture->handle());

    vec2 texture_size = framebuffer->size();
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

static auto draw_cubemap_mode() -> UnfoldedOrEquirectangular {
    static UnfoldedOrEquirectangular mode = Equirectangular;

    char const* mode_text = mode == Unfolded ? "Unfolded" : "Equirectangular projection";

    if (ImGui::BeginCombo("View", mode_text)) {
        if (ImGui::Selectable("Unfolded", mode == Unfolded)) {
            mode = Unfolded;
        }
        if (ImGui::Selectable("Equirectangular projection", mode == Equirectangular)) {
            mode = Equirectangular;
        }

        ImGui::EndCombo();
    }

    return mode;
}

static auto draw_cubemap(Texture* texture) -> void {
    auto cubemap_mode = draw_cubemap_mode();
    auto* drawn_cubemap = ui::internal::draw_cubemap_to_framebuffer(*texture, cubemap_mode);
    draw_texture_2d(drawn_cubemap, drawn_cubemap->color());
}

auto ui::internal::draw_framebuffer_viewer() -> void {
    auto& framebuffer_cache = entt::locator<FramebufferCache>::value();

    static u32 current_index = static_cast<u32>(-1);

    draw_selector(current_index, framebuffer_cache);
    ColorOrDepth mode = draw_color_or_depth();

    if (current_index == static_cast<u32>(-1)) {
        return;
    }

    auto const& framebuffer = framebuffer_cache[current_index];

    Texture* texture = nullptr;
    if (mode == Color) {
        texture = framebuffer->color();
    } else if (mode == Depth) {
        texture = framebuffer->depth();
    }

    if (texture == nullptr) {
        return;
    }

    if (texture->type() == Texture::Type::CubeMap) {
        // render cubemap
        draw_cubemap(texture);
    } else {
        // render texture 2d
        draw_texture_2d(framebuffer.handle().get(), texture);
    }
}