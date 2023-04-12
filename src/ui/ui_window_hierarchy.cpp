#include <imgui/imgui.h>

#include <entt/entt.hpp>

#include "../components/base_component.h"
#include "../core/editor_selected_entity.h"
#include "../core/entity.h"
#include "ui_internal.h"

using namespace engine;

static auto hierarchy_item(
    entt::registry& registry,
    entt::entity entity,
    BaseComponent const& base,
    entt::entity currently_selected
) -> void {
    ImGuiTreeNodeFlags const tree_base_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags const tree_leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGuiTreeNodeFlags const tree_selected_flags = ImGuiTreeNodeFlags_Selected;

    ImGuiTreeNodeFlags flags = tree_base_flags;
    bool is_leaf = false;

    if (base.first_child == entt::null) {
        flags |= tree_leaf_flags;
        is_leaf = true;
    }

    if (entity == currently_selected) {
        flags |= tree_selected_flags;
    }

    bool is_open = ImGui::TreeNodeEx(reinterpret_cast<void*>(entity), flags, "%s", base.name.c_str());

    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Duplicate")) {
            set_editor_selected_entity(registry, duplicate_entity(registry, entity));
        }

        ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        set_editor_selected_entity(registry, entity);
    }

    if (is_open && !is_leaf) {
        entt::entity child = base.first_child;

        while (child != entt::null) {
            auto& child_base = registry.get<BaseComponent>(child);

            hierarchy_item(registry, child, child_base, currently_selected);

            child = child_base.next_sibling;
        }

        ImGui::TreePop();
    }
}

auto ui::internal::ui_draw_window_hierarchy(entt::registry& registry, entt::entity currently_selected) -> void {
    ImGui::Begin("Hierarchy");

    auto view = registry.view<BaseComponent>();

    for (auto [entity, base] : view.each()) {
        if (base.parent != entt::null) {
            // We show only top-level entities
            // child entities will be shown later.
            continue;
        }

        hierarchy_item(registry, entity, base, currently_selected);
    }

    ImGui::End();
}