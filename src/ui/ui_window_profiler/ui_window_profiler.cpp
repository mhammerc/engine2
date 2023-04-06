#include <chrono>

#include "../../core/profiler.h"
#include "../imgui.h"
#include "../ui_internal.h"
#include "imgui/imgui.h"

using namespace engine;

static ImGuiTreeNodeFlags const tree_base_flags =
    ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
static ImGuiTreeNodeFlags const tree_leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

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

    auto duration =
        std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(block->end_time - block->start_time);

    auto duration_text = fmt::format("{:.2}ms", duration.count());
    ImGui::SameLine();
    ImGui::SmallButton(duration_text.c_str());

    if (is_open && !is_leaf) {
        for (auto& children_block : block->childrens) {
            draw_profiler_block(&children_block);
        }

        ImGui::TreePop();
    }
}

auto ui::internal::ui_draw_window_profiler(bool* is_open) -> void {
    if (!*is_open) {
        return;
    }

    auto& profiler = entt::locator<Profiler>::value();

    ImGui::Begin("Profiler", is_open);

    if (ImGui::Button("Profile again")) {
        profiler.enable_for_next_frame();
    }

    auto* root_block = profiler.root_block();

    if (!root_block || root_block->childrens.empty()) {
        ImGui::Text("Nothing have been profiled yet.");
    } else {
        draw_profiler_block(root_block);
    }

    ImGui::End();
}