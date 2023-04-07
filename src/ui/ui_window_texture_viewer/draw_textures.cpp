#include "../imgui.h"
#include "ui_window_texture_viewer.h"

using namespace engine;

static auto draw_texture_list(std::span<Texture*> const& textures, u32& current_index) -> void {
    auto const preview_name = current_index < textures.size() ? textures[current_index]->name() : "Please select";

    if (ImGui::BeginCombo("Texture", preview_name.c_str())) {
        for (u32 index = 0; auto* texture : textures) {
            auto selectable_name = fmt::format("{}##{}", texture->name(), static_cast<void*>(texture));

            if (ImGui::Selectable(selectable_name.c_str(), index == current_index)) {
                current_index = index;
            }

            ++index;
        }

        ImGui::EndCombo();
    }
}

auto ui::internal::draw_textures(std::span<Texture*> const& textures, u32& current_index) -> void {
    if (textures.empty()) {
        return;
    }

    if (current_index != static_cast<u32>(-1) && current_index >= textures.size()) {
        current_index = static_cast<u32>(-1);
    }

    draw_texture_list(textures, current_index);

    if (current_index != static_cast<u32>(-1)) {
        Texture* texture = textures[current_index];

        draw_texture(texture);
    }
}