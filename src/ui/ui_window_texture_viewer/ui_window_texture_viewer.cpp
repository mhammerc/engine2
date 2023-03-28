#include "ui_window_texture_viewer.h"

#include <imgui/imgui.h>

#include "../ui_internal.h"

using namespace engine;

auto ui::internal::ui_draw_window_texture_viewer(bool* is_open) -> void {
    if (!*is_open) {
        return;
    }

    ImGui::Begin("Texture Viewer", is_open);

    if (ImGui::BeginTabBar("FramebufferViewerTabbar")) {
        if (ImGui::BeginTabItem("Deferred Renderer")) {
            draw_deferred_renderer_viewer();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("FrameBuffer")) {
            draw_framebuffer_viewer();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Texture")) {
            draw_texture_viewer();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Cubemap")) {
            draw_cubemap_viewer();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}