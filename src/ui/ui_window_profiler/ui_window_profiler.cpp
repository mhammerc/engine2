#include <chrono>

#include "../../core/profiler.h"
#include "../imgui.h"
#include "../ui_internal.h"
#include "imgui/imgui.h"

using namespace engine;

static ImGuiTreeNodeFlags const tree_base_flags = ImGuiTreeNodeFlags_SpanAvailWidth;
static ImGuiTreeNodeFlags const tree_leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

// Properly format duration with proper units. Only works for seconds and milliseconds.
static auto format_duration(std::chrono::duration<float, std::milli> duration) -> std::string {
    if (duration.count() >= 1000) {
        using seconds = std::ratio<1LL, 1LL>;
        auto duration_seconds = std::chrono::duration_cast<std::chrono::duration<float, seconds>>(duration);

        return fmt::format("{:.3g}s", duration_seconds.count());
    }

    return fmt::format("{:.3g}ms", duration.count());
}

static auto draw_profiler_block(Profiler::Block* block) -> void {
    bool is_leaf = false;
    if (block->childrens.empty()) {
        is_leaf = true;
    }

    ImGuiTreeNodeFlags flags = tree_base_flags;
    if (is_leaf) {
        flags |= tree_leaf_flags;
    }

    bool is_open = ImGui::TreeNodeEx(block->name.c_str(), flags);

    if (block->end_time.time_since_epoch().count() != 0) {
        auto duration = format_duration(block->end_time - block->start_time);
        ImGui::SameLine();
        ImGui::SmallButton(duration.c_str());
    } else {
        ImGui::SameLine();
        ImGui::SmallButton("Still ongoing");
    }

    if (is_open && !is_leaf) {
        for (auto& children_block : block->childrens) {
            draw_profiler_block(&children_block);
        }

        ImGui::TreePop();
    }
}

static auto draw_profiler(Profiler* profiler, bool start_button) -> void {
    if (start_button && ImGui::Button("Profile again")) {
        profiler->enable_for_next_frame();
    }

    auto* root_block = profiler->root_block();

    if (!root_block || root_block->childrens.empty()) {
        ImGui::Text("Nothing have been profiled yet.");
    } else {
        draw_profiler_block(root_block);
    }
}

auto ui::internal::ui_draw_window_profiler(bool* is_open) -> void {
    if (!*is_open) {
        return;
    }

    ImGui::Begin("Profiler", is_open);

    if (ImGui::BeginTabBar("ProfilerTabBar")) {
        if (ImGui::BeginTabItem("Frame Profiler")) {
            auto& frame_profiler = entt::locator<ProfilerFrame>::value();

            draw_profiler(&frame_profiler, true);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Permanent Profiler")) {
            auto& permanent_profiler = entt::locator<ProfilerPermanent>::value();

            draw_profiler(&permanent_profiler, false);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}