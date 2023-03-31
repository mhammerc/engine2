#include <entt/entt.hpp>
#include <ranges>

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

auto ui::internal::draw_framebuffer_viewer() -> void {
    auto& framebuffer_cache = entt::locator<FramebufferCache>::value();

    static u32 framebuffer_index = static_cast<u32>(-1);
    draw_selector(framebuffer_index, framebuffer_cache);

    if (framebuffer_index == static_cast<u32>(-1)) {
        return;
    }
    auto framebuffer = framebuffer_cache[framebuffer_index];

    static u32 texture_index = static_cast<u32>(-1);
    draw_framebuffer(framebuffer.handle().get(), texture_index);
}