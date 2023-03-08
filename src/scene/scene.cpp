#include "scene.h"

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <entt/entt.hpp>

#include "../core/entity.h"
#include "../graphics/cube_map_cache.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/renderer_context.h"
#include "../graphics/shader_cache.h"
#include "components/base_component.h"
#include "components/camera_component.h"
#include "components/light_component.h"
#include "components/material_component.h"
#include "components/player_component.h"
#include "components/skybox_component.h"
#include "entity_from_model.h"

using namespace engine;

Scene::Scene(entt::registry& registry) : registry(registry) {
    auto backpacks = entity_from_model("../assets/backpack/backpack.obj", registry);
    for (auto& backpack : backpacks) {
        auto& base = registry.emplace<BaseComponent>(backpack, "backpack 1");
        base.transform.position = vec3(0.F, 0.F, -5.F);
    }

    auto duplicated_backpack = duplicate_entity(registry, backpacks[0]);
    registry.get<BaseComponent>(duplicated_backpack).name = "backpack 2";
    registry.get<BaseComponent>(duplicated_backpack).parent = backpacks[0];
    registry.get<BaseComponent>(backpacks[0]).first_child = duplicated_backpack;

    auto floors = entity_from_model("../assets/plane/plane.obj", registry);
    for (auto& floor : floors) {
        auto& base = registry.emplace<BaseComponent>(floor, "floor");
        base.transform.position = vec3(0.F, -5.F, 0.F);
    }

    auto skybox = registry.create();
    {
        registry.emplace<BaseComponent>(skybox, "skybox");
        registry.emplace<SkyboxComponent>(skybox);
    }

    camera = registry.create();
    auto& base = registry.emplace<BaseComponent>(camera, "player");
    auto& player = registry.emplace<CameraComponent>(camera);
    registry.emplace<PlayerComponent>(camera);
    base.transform.position = vec3(-8.3F, 0.F, 2.3F);
    player.pitch = 0.F;
    player.yaw = 140.F;
    player.update_base_rotation(base);

    auto& spotlight = registry.emplace<LightComponent>(camera);
    spotlight.type = LightComponent::Spot;
    spotlight.inner_cut_off = glm::cos(glm::radians(12.5F));
    spotlight.outer_cut_off = glm::cos(glm::radians(17.5F));
    spotlight.linear = 0.09F;
    spotlight.quadratic = 0.032F;
    spotlight.ambient = glm::vec3(0.2F, 0.2F, 0.2F);
    spotlight.diffuse = glm::vec3(0.5F, 0.5F, 0.5F);
    spotlight.specular = glm::vec3(0.0F, 0.0F, 0.0F);
    spotlight.draw_gizmo = false;
}

auto Scene::camera_info() -> std::tuple<engine::BaseComponent&, engine::CameraComponent&> {
    return registry.get<BaseComponent, CameraComponent>(camera);
}

auto Scene::draw(float /*delta_time*/) -> void {
    if (outline) {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    draw_nodes();

    if (show_normals) {
        draw_nodes_normals();
    }

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glDisable(GL_STENCIL_TEST);
    draw_skybox();

    if (outline) {
        draw_nodes_outline();
    }
}

auto Scene::draw_nodes() -> void {
    auto drawables = registry.view<BaseComponent, MaterialComponent>();
    auto lights = registry.view<BaseComponent, LightComponent>();

    auto& cubemap_cache = entt::locator<CubeMapCache>::value();
    auto skybox = cubemap_cache["skybox"_hs];
    skybox->activate_as(10);

    auto [camera_base, camera_config] = camera_info();

    for (auto [entity, base, material] : drawables.each()) {
        if (!base.enabled) {
            continue;
        }

        auto const& renderer_context = entt::locator<RendererContext>::value();

        auto const model = base.world_matrix(registry);
        auto const modelNormal = mat3(glm::transpose(glm::inverse(model)));

        // MVP
        material.shader->set_uniform("model", model);
        material.shader->set_uniform("modelNormal", modelNormal);
        material.shader->set_uniform("view", camera_config.view_matrix(camera_base));
        material.shader->set_uniform("projection", renderer_context.projection);

        material.shader->set_uniform("cameraPosition", camera_base.world_transform(registry).position);

        int light_index = 0;
        for (auto [entity, base, light] : lights.each()) {
            if (!base.enabled) {
                continue;
            }

            material.shader->set_uniform(fmt::format("lights[{}].type", light_index), static_cast<int>(light.type));
            material.shader->set_uniform(
                fmt::format("lights[{}].position", light_index),
                base.world_transform(registry).position
            );
            material.shader->set_uniform(
                fmt::format("lights[{}].direction", light_index),
                base.world_transform(registry).direction()
            );
            material.shader->set_uniform(fmt::format("lights[{}].innerCutOff", light_index), light.inner_cut_off);
            material.shader->set_uniform(fmt::format("lights[{}].outerCutOff", light_index), light.outer_cut_off);
            material.shader->set_uniform(fmt::format("lights[{}].constant", light_index), light.constant);
            material.shader->set_uniform(fmt::format("lights[{}].linear", light_index), light.linear);
            material.shader->set_uniform(fmt::format("lights[{}].quadratic", light_index), light.quadratic);
            material.shader->set_uniform(fmt::format("lights[{}].ambient", light_index), light.ambient);
            material.shader->set_uniform(fmt::format("lights[{}].diffuse", light_index), light.diffuse);
            material.shader->set_uniform(fmt::format("lights[{}].specular", light_index), light.specular);
            light_index += 1;
        }

        material.shader->set_uniform("material.shininess", material.shininess);
        material.shader->set_uniform("material.reflect", material.reflection);
        material.shader->set_uniform("material.refract", material.refraction);
        material.shader->set_uniform("material.texture_environment", 10);

        if (material.textures.contains(1)) {
            material.shader->set_uniform("material.texture_diffuse1", 1);
        } else {
            material.shader->set_uniform("material.texture_diffuse1", 0);
        }

        if (material.textures.contains(2)) {
            material.shader->set_uniform("material.texture_specular1", 2);
        } else {
            material.shader->set_uniform("material.texture_specular1", 0);
        }

        material.shader->set_uniform("explosion", explosion);

        material.shader->bind();
        for (auto const& texture : material.textures) {
            texture.second->activate_as(texture.first, false);
        }

        material.mesh->draw();
    }

    skybox->activate_as(10, true);
}

auto Scene::draw_nodes_outline() -> void {
    auto [camera_base, camera_config] = camera_info();
    auto shader_cache = entt::locator<ShaderCache>::value();
    auto shader_outline = shader_cache["outline"_hs];

    auto drawables = registry.view<BaseComponent, MaterialComponent>();

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    for (auto [entity, base, material] : drawables.each()) {
        if (!base.enabled) {
            continue;
        }

        auto const& renderer_context = entt::locator<RendererContext>::value();

        auto const model = base.world_matrix(registry);
        auto const modelNormal = mat3(glm::transpose(glm::inverse(model)));

        // MVP
        shader_outline->set_uniform("model", model);
        shader_outline->set_uniform("modelNormal", modelNormal);
        shader_outline->set_uniform("view", camera_config.view_matrix(camera_base));
        shader_outline->set_uniform("projection", renderer_context.projection);

        shader_outline->bind();
        material.mesh->draw();
        shader_outline->unbind();
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
}

auto Scene::draw_nodes_normals() -> void {
    auto [camera_base, camera_config] = camera_info();
    auto shader_cache = entt::locator<ShaderCache>::value();
    auto shader_normal = shader_cache["normal"_hs];

    auto const& renderer_context = entt::locator<RendererContext>::value();

    auto drawables = registry.view<BaseComponent, MaterialComponent>();

    for (auto [entity, base, material] : drawables.each()) {
        if (!base.enabled) {
            continue;
        }

        auto const model = base.world_matrix(registry);
        auto const modelNormal = mat3(glm::transpose(glm::inverse(model)));

        // MVP
        shader_normal->set_uniform("model", model);
        shader_normal->set_uniform("modelNormal", modelNormal);
        shader_normal->set_uniform("view", camera_config.view_matrix(camera_base));
        shader_normal->set_uniform("projection", renderer_context.projection);

        shader_normal->bind();
        material.mesh->draw();
        shader_normal->unbind();
    }
}

auto Scene::draw_skybox() -> void {
    auto [camera_base, camera_config] = camera_info();
    glDepthFunc(GL_LEQUAL);

    auto skyboxes = registry.view<BaseComponent, SkyboxComponent>();

    auto const& renderer_context = entt::locator<RendererContext>::value();

    auto cubemap = entt::locator<CubeMapCache>::value()["skybox"_hs];
    auto shader = entt::locator<ShaderCache>::value()["skybox"_hs];
    auto cube = entt::locator<MeshCache>::value()["cube"_hs];

    cubemap->activate_as(0);
    shader->set_uniform("skybox", 0);

    auto view = camera_config.view_matrix(camera_base);
    // Remove translation from the view matrix.
    // For the skybox, only rotation is needed.
    view = glm::mat4(glm::mat3(view));
    shader->set_uniform("view", view);
    shader->set_uniform("projection", renderer_context.projection);

    shader->bind();
    glCullFace(GL_FRONT);

    for (auto [entity, name] : skyboxes.each()) {
        if (!name.enabled) {
            continue;
        }

        cube->draw();
    }

    glCullFace(GL_BACK);
    shader->unbind();
    cubemap->activate_as(0, true);

    glDepthFunc(GL_LESS);
}
