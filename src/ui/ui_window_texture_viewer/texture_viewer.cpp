#include <spdlog/fmt/fmt.h>

#include <entt/entt.hpp>
#include <optional>

#include "../../common.h"
#include "../../graphics/texture_cache.h"
#include "../imgui.h"
#include "../ui_internal.h"
#include "ui_window_texture_viewer.h"

using namespace engine;

auto ui::internal::texture() -> void {
    auto& texture_cache = entt::locator<TextureCache>::value();
    static u32 current_index = static_cast<u32>(-1);

    {
        auto current =
            texture_cache.contains(current_index) ? std::make_optional(texture_cache[current_index]) : std::nullopt;

        auto const preview_name = current.has_value() ? (*current)->name() : "Please select";

        if (ImGui::BeginCombo("Texture", preview_name.c_str())) {
            for (auto texture : texture_cache) {
                auto index = texture.first;
                auto const name = texture.second->name();

                if (ImGui::Selectable(fmt::format("{}##{}", name, index).c_str(), index == current_index)) {
                    current_index = index;
                }
            }

            ImGui::EndCombo();
        }
    }

    if (current_index == static_cast<u32>(-1)) {
        return;
    }

    auto const& texture = texture_cache[current_index];
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