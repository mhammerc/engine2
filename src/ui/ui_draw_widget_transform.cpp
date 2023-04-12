#include "imgui.h"
#include "imgui/ImGuizmo.h"
#include "imgui/imgui.h"
#include "ui_internal.h"

using namespace engine;

static auto push_enabled_button_style(bool cond = true) -> void {
    if (!cond) {
        return;
    }

    ImGui::PushStyleColor(ImGuiCol_Button, vec4 {0.06F, 0.53F, 0.98F, 1.F});
}

static auto pop_enabled_button_style(bool cond = true) -> void {
    if (!cond) {
        return;
    }

    ImGui::PopStyleColor(1);
}

auto engine::ui::internal::ui_draw_widget_transform() -> std::pair<ImGuizmo::OPERATION, ImGuizmo::MODE> {
    static auto operation = ImGuizmo::OPERATION::TRANSLATE;
    static auto mode = ImGuizmo::MODE::WORLD;

    bool is_translate = operation == ImGuizmo::OPERATION::TRANSLATE;
    bool is_rotate = operation == ImGuizmo::OPERATION::ROTATE;
    bool is_scale = operation == ImGuizmo::OPERATION::SCALE;
    bool is_world = mode == ImGuizmo::MODE::WORLD;
    bool is_local = mode == ImGuizmo::MODE::LOCAL;

    if (ImGui::IsWindowFocused() && ImGui::IsKeyDown(ImGuiKey_W)) {
        operation = ImGuizmo::OPERATION::TRANSLATE;
    }

    if (ImGui::IsWindowFocused() && ImGui::IsKeyDown(ImGuiKey_E)) {
        operation = ImGuizmo::OPERATION::ROTATE;
    }

    if (ImGui::IsWindowFocused() && ImGui::IsKeyDown(ImGuiKey_R)) {
        operation = ImGuizmo::OPERATION::SCALE;
    }

    ImGui::Indent(6);

    push_enabled_button_style(is_translate);
    if (ImGui::SmallButton("T")) {
        operation = ImGuizmo::OPERATION::TRANSLATE;
    }
    pop_enabled_button_style(is_translate);

    ImGui::SameLine();

    push_enabled_button_style(is_rotate);
    if (ImGui::SmallButton("R")) {
        operation = ImGuizmo::OPERATION::ROTATE;
    }
    pop_enabled_button_style(is_rotate);

    ImGui::SameLine();

    push_enabled_button_style(is_scale);
    if (ImGui::SmallButton("S")) {
        operation = ImGuizmo::OPERATION::SCALE;
    }
    pop_enabled_button_style(is_scale);

    push_enabled_button_style(is_world);
    if (ImGui::SmallButton("World")) {
        mode = ImGuizmo::MODE::WORLD;
    }
    pop_enabled_button_style(is_world);

    ImGui::SameLine();

    push_enabled_button_style(is_local);
    if (ImGui::SmallButton("Local")) {
        mode = ImGuizmo::MODE::LOCAL;
    }
    pop_enabled_button_style(is_local);

    return {operation, mode};
}