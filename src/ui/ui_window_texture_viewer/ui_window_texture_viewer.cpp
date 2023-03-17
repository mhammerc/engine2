#include "ui_window_texture_viewer.h"

#include <imgui/imgui.h>

#include <entt/entt.hpp>
#include <optional>

#include "../ui_internal.h"

using namespace engine;

auto ui::internal::ui_draw_window_texture_viewer(bool* is_open) -> void {
    if (!*is_open) {
        return;
    }

    ImGui::Begin("Texture Viewer", is_open);

    if (ImGui::BeginTabBar("FramebufferViewerTabbar")) {
        if (ImGui::BeginTabItem("FrameBuffer")) {
            framebuffer();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Texture")) {
            texture();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Cubemap")) {
            cubemap();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}