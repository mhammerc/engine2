#include <imgui/imgui.h>

#include <entt/entt.hpp>

#include "../core/entity.h"
#include "../scene/components/name_component.h"
#include "ui_internal.h"

using namespace engine;

static auto hierarchy_item(
    entt::registry& registry,
    entt::entity entity,
    NameComponent const& name,
    entt::entity& currently_selected
) -> void {
    ImGuiTreeNodeFlags const tree_base_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags const tree_leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGuiTreeNodeFlags const tree_selected_flags = ImGuiTreeNodeFlags_Selected;

    ImGuiTreeNodeFlags flags = tree_base_flags | tree_leaf_flags;

    if (entity == currently_selected) {
        flags |= tree_selected_flags;
    }

    // ImGui::TreeNodeEx((void*)(intptr_t)entity, flags, "%s", name.name.c_str());
    ImGui::TreeNodeEx(reinterpret_cast<void*>(entity), flags, "%s", name.name.c_str());
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Duplicate")) {
            currently_selected = duplicate_entity(registry, entity);
        }

        ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        currently_selected = entity;
    }
}

auto ui::internal::ui_draw_window_hierarchy(entt::registry& registry, entt::entity& currently_selected) -> void {
    ImGui::Begin("Hierarchy");

    auto view = registry.view<NameComponent>();

    for (auto [entity, name] : view.each()) {
        hierarchy_item(registry, entity, name, currently_selected);
    }

    ImGui::End();
}