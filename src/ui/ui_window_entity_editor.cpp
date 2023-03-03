#include <imgui/imgui.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "../graphics/mesh.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/shader_cache.h"
#include "../graphics/shader_program.h"
#include "../scene/components/name_component.h"
#include "spdlog/fmt/bundled/core.h"
#include "ui_internal.h"
#include "utils.h"

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

template<typename T, typename meta>
static auto prop(std::string_view name, meta const& type) -> T {
    if (auto prop = type.prop(entt::hashed_string {name.data()}); prop) {
        auto value = prop.value().template cast<T>();

        if (value) {
            return value;
        }
    }

    return nullptr;
}

static auto on_property(entt::meta_any& instance, entt::meta_data const& member) -> void {
    auto const* property_name = prop<char const*>("name", member);

    if (!property_name) {
        return;
    }

    if (auto any = member.get(instance)) {
        if (auto* value = any.try_cast<float>(); value) {
            bool edited = ImGui::DragFloat(property_name, value, 0.25F);

            if (edited) {
                member.set(instance, *value);
            }
        }

        if (auto* value = any.try_cast<vec3>(); value) {
            bool edited = ImGui::DragFloat3(property_name, &value->x, 0.25F);

            if (edited) {
                member.set(instance, *value);
            }
        }

        if (auto* value = any.try_cast<std::shared_ptr<ShaderProgram>>(); value) {
            auto shader_cache = entt::locator<ShaderCache>::value();

            if (ImGui::BeginCombo("Shader", (*value)->name().data())) {
                for (auto shader : shader_cache) {
                    bool selected = ImGui::Selectable(
                        fmt::format("{}##{}", shader.second->name(), shader.first).c_str(),
                        shader.second.handle().get() == value->get()
                    );

                    if (selected) {
                        member.set(instance, shader.second.handle());
                    }
                }
                ImGui::EndCombo();
            }
        }

        if (auto* value = any.try_cast<std::shared_ptr<Mesh>>(); value) {
            auto shader_cache = entt::locator<MeshCache>::value();

            if (ImGui::BeginCombo("Mesh", (*value)->name().data())) {
                for (auto shader : shader_cache) {
                    bool selected = ImGui::Selectable(
                        fmt::format("{}##{}", shader.second->name(), shader.first).c_str(),
                        shader.second.handle().get() == value->get()
                    );

                    if (selected) {
                        member.set(instance, shader.second.handle());
                    }
                }
                ImGui::EndCombo();
            }
        }

        if (auto* value = any.try_cast<std::map<i32, std::shared_ptr<Texture>>>(); value) {
            ImGui::Text("texture");
        }
    }
}

static auto on_component(entt::meta_any& instance, entt::meta_type& type) -> void {
    bool open = false;

    if (auto const* name = prop<char const*>("name", type); name) {
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
            on_property(instance, member);
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