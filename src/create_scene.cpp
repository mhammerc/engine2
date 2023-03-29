#include "create_scene.h"

#include "components/base_component.h"
#include "components/camera_component.h"
#include "components/light_component.h"
#include "components/outline_component.h"
#include "components/skybox_component.h"
#include "core/entity.h"
#include "entity_from_model.h"

using namespace engine;

auto engine::create_scene(entt::registry& registry, RendererContext& renderer_context) -> void {
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

    renderer_context.camera = registry.create();
    auto& base = registry.emplace<BaseComponent>(renderer_context.camera, "player");
    auto& player = registry.emplace<CameraComponent>(renderer_context.camera);
    base.transform.position = vec3(-8.3F, 0.F, 2.3F);
    player.pitch = 0.F;
    player.yaw = 140.F;
    player.update_base_rotation(base);

    auto light1 = registry.create();
    auto& light1_base = registry.emplace<BaseComponent>(light1, "light1");
    auto& light1_light = registry.emplace<LightComponent>(light1);
    light1_base.transform.position = vec3(0.F, 0.F, -2.F);
    light1_light.type = LightComponent::Point;
    light1_light.linear = 0.09F;
    light1_light.quadratic = 0.032F;
    light1_light.ambient = glm::vec3(0.35F, 0.35F, 0.35F);
    light1_light.diffuse = glm::vec3(0.8F, 0.8F, 0.8F);
    light1_light.specular = glm::vec3(1.F, 1.F, 1.F);

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