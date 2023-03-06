#include <imgui/imgui.h>

#include "../core/input.hpp"
#include "ui_internal.h"

using namespace engine;

auto ui::internal::ui_draw_input_debugger(bool* is_open) -> void {
    if (!*is_open) {
        return;
    }

    auto const& input = entt::locator<Input>::value();

    ImGui::Begin("Input Debugger", is_open);

    auto mouse_position = input.mouse_position();
    ImGui::Text("Mouse Position: (%.3f,%.3f)", mouse_position.x, mouse_position.y);

    ImGui::Text("List of pressed keys:");
    for (size i = 0; i < input.keys.size(); ++i) {
        if (input.keys[i]) {
            ImGui::Text("%s", Input::get_key_name(static_cast<int>(i)));
        }
    }

    ImGui::End();
}