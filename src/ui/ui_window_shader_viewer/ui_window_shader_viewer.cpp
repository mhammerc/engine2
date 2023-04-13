#include "ui_window_shader_viewer.h"

#include "../../graphics/shader_cache.h"
#include "../imgui.h"
#include "../ui_internal.h"
#include "imgui/imgui.h"

using namespace engine;

static auto draw_shaders_list() -> ShaderProgram* {
    auto& shader_cache = entt::locator<ShaderCache>::value();

    static ShaderProgram* selected_shader = nullptr;

    const char* selected_name = selected_shader ? selected_shader->name().data() : "Please select";

    if (ImGui::BeginCombo("Shader", selected_name)) {
        for (auto [index, shader] : shader_cache) {
            bool selected = ImGui::Selectable(
                fmt::format("{}##{}", shader->name(), index).c_str(),
                selected_shader == shader.handle().get()
            );

            if (selected) {
                selected_shader = shader.handle().get();
            }
        }

        ImGui::EndCombo();
    }

    return selected_shader;
}

static auto draw_shader_options(ShaderProgram* shader) -> void {
    if (!shader) {
        return;
    }

    static std::optional<bool> reload_success = std::nullopt;

    {
        static ShaderProgram const* previous_shader = shader;
        if (previous_shader != shader) {
            reload_success = std::nullopt;
        }
        previous_shader = shader;
    }

    if (ImGui::Button("Reload source files")) {
        reload_success = shader->reload();
    }

    if (reload_success.has_value() && *reload_success) {
        ImGui::TextColored({0.F, 1.F, 0.F, 1.F}, "Reload Success");
    } else if (reload_success.has_value()) {
        ImGui::TextColored({1.F, 0.F, 0.F, 1.F}, "Reload Fail");
    }
}

auto ui::internal::ui_draw_window_shader_viewer(bool* is_open) -> void {
    if (!*is_open) {
        return;
    }

    ImGui::Begin("Shader Viewer", is_open);

    auto* selected_shader = draw_shaders_list();

    if (selected_shader) {
        draw_shader_options(selected_shader);
    }

    ImGui::End();
}