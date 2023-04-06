#include <fmt/core.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include "../../components/base_component.h"
#include "../../components/outline_component.h"
#include "../../components/skybox_component.h"
#include "../ui_internal.h"
#include "../utils.h"
#include "entity_editor.h"

using namespace engine;

static auto header(entt::registry& registry, entt::entity entity) -> void {
    auto* name = registry.try_get<BaseComponent>(entity);

    if (name) {
        ImGui::Checkbox("##enabled", &name->enabled);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ui::internal::input_text("##name", &name->name);

        ImGui::DragFloat3("Position", &name->transform.position.x, 0.25F);
        ui::internal::on_property_quaternion("Rotation", &name->transform.rotation);
        ImGui::DragFloat3("Scale", &name->transform.scale.x, 0.25F);
    }
}

static auto on_component(entt::meta_any& instance, entt::meta_type& type) -> void {
    bool open = false;

    if (auto const* name = ui::internal::prop<char const*>("name", type); name) {
        if (strcmp("BaseComponent", name) == 0) {
            return;
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        open = ImGui::CollapsingHeader(name);
    } else {
        // The component does not have a name.
        open = ImGui::CollapsingHeader("Unknown Component");
        return;
    }

    if (open) {
        for (auto [_, member] : type.data()) {
            ui::internal::on_property(instance, member);
        }
    }
}

static auto iterate_components(entt::registry& registry, entt::entity entity) -> void {
    for (auto&& storage : registry.storage()) {
        auto reflection = entt::resolve(storage.first);

        if (!storage.second.contains(entity)) {
            continue;
        }

        auto* instance_address = storage.second.get(entity);

        if (!instance_address) {
            continue;
        }

        auto instance = reflection.from_void(instance_address);

        on_component(instance, reflection);
    }
}

/**
 * Empty components are treated differently because they are optimized away by EnTT.
 *
 * Because of this, we can not iterate on them like we do in `iterate_components`.
 *
 * For now this mean checking for hard-coded components.
 */
template<typename T>
static auto empty_component(entt::registry& registry, entt::entity entity) -> void {
    if (registry.all_of<T>(entity)) {
        auto reflection = entt::resolve<T>();

        if (auto const* name = ui::internal::prop<char const*>("name", reflection); name) {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            ImGui::CollapsingHeader(name);
        }
    }
}

static auto iterate_empty_components(entt::registry& registry, entt::entity entity) -> void {
    empty_component<OutlineComponent>(registry, entity);
}

auto ui::internal::ui_draw_window_entity_editor(entt::registry& registry, entt::entity entity) -> void {
    ImGui::Begin("Entity");

    if (entity != entt::null) {
        header(registry, entity);

        iterate_components(registry, entity);
        iterate_empty_components(registry, entity);
    }

    ImGui::End();
}