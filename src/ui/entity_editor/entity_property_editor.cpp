#include <fmt/core.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include "../../common.h"
#include "../../graphics/mesh.h"
#include "../../graphics/mesh_cache.h"
#include "../../graphics/renderer_context.h"
#include "../../graphics/shader_cache.h"
#include "../../graphics/shader_program.h"
#include "../../graphics/texture/texture.h"
#include "../../graphics/texture/texture_cache.h"
#include "entity_editor.h"
#include "glm/gtc/quaternion.hpp"

using namespace engine;

static auto on_property_float(const char* name, float* value) -> bool {
    bool edited = ImGui::DragFloat(name, value, 0.25F);

    return edited;
}

static auto on_property_float2(const char* name, vec2* value) -> bool {
    bool edited = ImGui::DragFloat2(name, &value->x, 0.25F);

    return edited;
}

static auto on_property_float3(const char* name, vec3* value) -> bool {
    bool edited = ImGui::DragFloat3(name, &value->x, 0.25F);

    return edited;
}

static auto on_property_float3_color(const char* name, vec3* value) -> bool {
    bool edited = ImGui::ColorEdit3(name, &value->x);

    return edited;
}

static auto on_property_boolean(const char* name, bool* value) -> bool {
    bool edited = ImGui::Checkbox(name, value);

    return edited;
}

auto ui::internal::on_property_quaternion(const char* name, glm::quat* value) -> bool {
    auto radians_euler_angles = glm::eulerAngles(*value);
    auto degree_euler_angles = glm::degrees(radians_euler_angles);

    if (on_property_float3(name, &degree_euler_angles)) {
        radians_euler_angles = glm::radians(degree_euler_angles);

        glm::quat new_quaternion(radians_euler_angles);

        *value = new_quaternion;

        return true;
    }

    return false;
}

static auto on_property_shader(const char* name, std::shared_ptr<ShaderProgram>* value)
    -> std::shared_ptr<ShaderProgram> {
    auto& shader_cache = entt::locator<ShaderCache>::value();
    std::shared_ptr<ShaderProgram> new_shader = nullptr;

    const char* selected_name = *value ? (*value)->name().data() : "Please select";

    if (ImGui::BeginCombo(name, selected_name)) {
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
    auto& mesh_cache = entt::locator<MeshCache>::value();
    std::shared_ptr<Mesh> new_mesh = nullptr;

    const char* selected_name = *value ? (*value)->name().data() : "Please select";

    if (ImGui::BeginCombo(name, selected_name)) {
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

static auto on_property_texture(const char* name, std::shared_ptr<Texture>* value) -> std::shared_ptr<Texture> {
    auto& texture_cache = entt::locator<TextureCache>::value();
    std::shared_ptr<Texture> new_texture = nullptr;

    const char* selected_name = *value ? (*value)->name().data() : "Please select";

    if (ImGui::BeginCombo(name, selected_name)) {
        for (auto texture : texture_cache) {
            bool selected = ImGui::Selectable(
                fmt::format("{}##{}", texture.second->name(), texture.first).c_str(),
                texture.second.handle().get() == value->get()
            );

            if (selected) {
                new_texture = texture.second.handle();
            }
        }
        ImGui::EndCombo();
    }

    return new_texture;
}

static auto on_property_tone_mapping(const char* name, RendererContext::ToneMapping* tone_mapping) -> bool {
    using ToneMapping = RendererContext::ToneMapping;

    auto tone_mapping_to_name = [](ToneMapping tone_mapping) -> char const* {
        if (tone_mapping == ToneMapping::AcesApprox) {
            return "Aces Approx";
        }
        if (tone_mapping == ToneMapping::AcesFitted) {
            return "Aces Fitted";
        }
        if (tone_mapping == ToneMapping::None) {
            return "No Tone Mapping (Disable HDR mapping)";
        }
        return "Missing case";
    };

    bool edited = false;

    auto tone_mapping_selectable = [&](ToneMapping option) {
        if (ImGui::Selectable(tone_mapping_to_name(option), *tone_mapping == option)) {
            *tone_mapping = option;
            edited = true;
        }
    };

    if (ImGui::BeginCombo(name, tone_mapping_to_name(*tone_mapping))) {
        tone_mapping_selectable(ToneMapping::AcesApprox);
        tone_mapping_selectable(ToneMapping::AcesFitted);
        tone_mapping_selectable(ToneMapping::None);

        ImGui::EndCombo();
    }

    return edited;
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

    MSVC_WARNING(disable : 4456)

    if (auto* value = any.try_cast<bool>(); value) {
        bool edited = on_property_boolean(property_name, value);

        if (edited) {
            member.set(instance, *value);
        }
    } else if (auto* value = any.try_cast<float>(); value) {
        bool edited = on_property_float(property_name, value);

        if (edited) {
            member.set(instance, *value);
        }
    } else if (auto* value = any.try_cast<vec2>(); value) {
        bool edited = on_property_float2(property_name, value);

        if (edited) {
            member.set(instance, *value);
        }
    } else if (auto* value = any.try_cast<vec3>(); value) {
        bool edited = false;
        char const* type = prop<char const*>("type", member);

        if (type && std::strcmp("color", type) == 0 && false) {
            // TODO: lights should have a power in lumen and a color.
            // color vec3
            edited = on_property_float3_color(property_name, value);
        } else {
            // regular vec3
            edited = on_property_float3(property_name, value);
        }

        if (edited) {
            member.set(instance, *value);
        }
    } else if (auto* value = any.try_cast<glm::quat>(); value) {
        bool edited = on_property_quaternion(property_name, value);

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
    } else if (auto* value = any.try_cast<std::shared_ptr<Texture>>(); value) {
        auto new_texture = on_property_texture(property_name, value);

        if (new_texture) {
            member.set(instance, new_texture);
        }
    } else if (auto* value = any.try_cast<RendererContext::ToneMapping>(); value) {
        auto new_tone_mapping = on_property_tone_mapping(property_name, value);

        if (new_tone_mapping) {
            member.set(instance, *value);
        }
    } else if (auto* value = any.try_cast<std::map<i32, std::shared_ptr<Texture>>>(); value) {
        ImGui::Text("texture");
    }

    MSVC_WARNING(default : 4456);
}