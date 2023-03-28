#include "scene.h"

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <entt/entt.hpp>

#include "../core/entity.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/renderer_context.h"
#include "../graphics/shader_cache.h"
#include "../graphics/texture/texture_cache.h"
#include "components/base_component.h"
#include "components/camera_component.h"
#include "components/outline_component.h"
#include "components/skybox_component.h"
#include "entity_from_model.h"

using namespace engine;

Scene::Scene(entt::registry& registry) : registry(registry) {
    auto backpacks = entity_from_model("../assets/backpack/backpack.obj", registry);
    for (auto& backpack : backpacks) {
        auto& base = registry.emplace<BaseComponent>(backpack, "backpack 1");
        base.transform.position = vec3(0.F, 0.F, -5.F);

        registry.emplace<OutlineComponent>(backpack);
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
    base.transform.position = vec3(-8.3F, 0.F, 2.3F);
    player.pitch = 0.F;
    player.yaw = 140.F;
    player.update_base_rotation(base);

    // auto& spotlight = registry.emplace<LightComponent>(camera);
    // spotlight.type = LightComponent::Spot;
    // spotlight.inner_cut_off = glm::cos(glm::radians(12.5F));
    // spotlight.outer_cut_off = glm::cos(glm::radians(17.5F));
    // spotlight.linear = 0.09F;
    // spotlight.quadratic = 0.032F;
    // spotlight.ambient = glm::vec3(0.2F, 0.2F, 0.2F);
    // spotlight.diffuse = glm::vec3(0.5F, 0.5F, 0.5F);
    // spotlight.specular = glm::vec3(0.0F, 0.0F, 0.0F);
    // spotlight.draw_gizmo = false;
}

auto Scene::camera_info() -> std::tuple<engine::BaseComponent&, engine::CameraComponent&> {
    return registry.get<BaseComponent, CameraComponent>(camera);
}

auto Scene::draw_skybox() -> void {
    auto [camera_base, camera_config] = camera_info();
    glDepthFunc(GL_LEQUAL);

    auto skyboxes = registry.view<BaseComponent, SkyboxComponent>();

    auto const& renderer_context = entt::locator<RendererContext>::value();

    auto cubemap = entt::locator<TextureCache>::value()["skybox"_hs];
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
