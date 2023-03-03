#include <imgui/imgui.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "../../scene/components/name_component.h"
#include "../ui_internal.h"
#include "../utils.h"
#include "entity_editor.h"

using namespace engine;

static auto header(entt::registry& registry, entt::entity entity) -> void {
    auto* name = registry.try_get<NameComponent>(entity);

    if (name) {
        ImGui::Checkbox("##enabled", &name->enabled);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ui::internal::input_text("##name", &name->name);
    }
}

static auto on_component(entt::meta_any& instance, entt::meta_type& type) -> void {
    bool open = false;

    if (auto const* name = ui::internal::prop<char const*>("name", type); name) {
        if (strcmp("NameComponent", name) == 0) {
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

auto ui::internal::ui_draw_window_entity_editor(entt::registry& registry, entt::entity entity) -> void {
    ImGui::Begin("Entity");

    if (entity != entt::null) {
        header(registry, entity);

        iterate_components(registry, entity);
    }

    ImGui::End();
}