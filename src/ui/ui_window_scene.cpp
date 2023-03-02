#include <imgui/imgui.h>

#include "ui_internal.h"

using namespace engine;

auto ui::internal::ui_draw_window_scene(FrameBuffer* scene_texture) -> void {
    ImGuiWindowFlags flags = ImGuiWindowFlags_None;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Scene", nullptr, flags);

    auto window_size = ImGui::GetContentRegionAvail();
    float dpi_scale = ImGui::GetWindowDpiScale();

    scene_texture->resize({window_size.x * dpi_scale, window_size.y * dpi_scale});

    ImGui::Image(
        reinterpret_cast<void*>(scene_texture->color_texture()->handle()),
        {window_size},
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::End();

    ImGui::PopStyleVar(1);
}