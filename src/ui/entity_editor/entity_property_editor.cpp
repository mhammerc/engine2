#include <imgui/imgui.h>
#include <spdlog/fmt/fmt.h>

#include "../../graphics/mesh.h"
#include "../../graphics/mesh_cache.h"
#include "../../graphics/shader_cache.h"
#include "../../graphics/shader_program.h"
#include "../../graphics/texture.h"
#include "entity_editor.h"

using namespace engine;

static auto on_property_float(const char* name, float* value) -> bool {
    bool edited = ImGui::DragFloat(name, value, 0.25F);

    return edited;
}

static auto on_property_float3(const char* name, vec3* value) -> bool {
    bool edited = ImGui::DragFloat3(name, &value->x, 0.25F);

    return edited;
}

static auto on_property_shader(const char* name, std::shared_ptr<ShaderProgram>* value)
    -> std::shared_ptr<ShaderProgram> {
    auto shader_cache = entt::locator<ShaderCache>::value();
    std::shared_ptr<ShaderProgram> new_shader = nullptr;

    if (ImGui::BeginCombo(name, (*value)->name().data())) {
        for (auto shader : shader_cache) {
            bool selected = ImGui::Selectable(
                fmt::format("{}##{}", shader.second->name(), shader.first).c_str(),
                shader.second.handle().get() == value->get()
            );

            if (selected) {
                new_shader = shader.second.handle();
            }
        }
        ImGui::EndCombo();
    }

    return new_shader;
}

static auto on_property_mesh(const char* name, std::shared_ptr<Mesh>* value) -> std::shared_ptr<Mesh> {
    auto mesh_cache = entt::locator<MeshCache>::value();
    std::shared_ptr<Mesh> new_mesh = nullptr;

    if (ImGui::BeginCombo(name, (*value)->name().data())) {
        for (auto mesh : mesh_cache) {
            bool selected = ImGui::Selectable(
                fmt::format("{}##{}", mesh.second->name(), mesh.first).c_str(),
                mesh.second.handle().get() == value->get()
            );

            if (selected) {
                new_mesh = mesh.second.handle();
            }
        }
        ImGui::EndCombo();
    }

    return new_mesh;
}

auto ui::internal::on_property(entt::meta_any& instance, entt::meta_data const& member) -> void {
    auto const* property_name = prop<char const*>("name", member);

    if (!property_name) {
        return;
    }

    auto any = member.get(instance);

    if (!any) {
        return;
    }

    if (auto* value = any.try_cast<float>(); value) {
        bool edited = on_property_float(property_name, value);

        if (edited) {
            member.set(instance, *value);
        }
    } else if (auto* value = any.try_cast<vec3>(); value) {
        bool edited = on_property_float3(property_name, value);

        if (edited) {
            member.set(instance, *value);
        }
    } else if (auto* value = any.try_cast<std::shared_ptr<ShaderProgram>>(); value) {
        auto new_shader = on_property_shader(property_name, value);

        if (new_shader) {
            member.set(instance, new_shader);
        }

    } else if (auto* value = any.try_cast<std::shared_ptr<Mesh>>(); value) {
        auto new_mesh = on_property_mesh(property_name, value);

        if (new_mesh) {
            member.set(instance, new_mesh);
        }
    } else if (auto* value = any.try_cast<std::map<i32, std::shared_ptr<Texture>>>(); value) {
        ImGui::Text("texture");
    }
}