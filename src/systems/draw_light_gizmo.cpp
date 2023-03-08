#include "../graphics/mesh_cache.h"
#include "../graphics/renderer_context.h"
#include "../graphics/shader_cache.h"
#include "../scene/components/base_component.h"
#include "../scene/components/camera_component.h"
#include "../scene/components/light_component.h"
#include "glm/ext/matrix_transform.hpp"
#include "systems.h"

using namespace engine;

auto systems::draw_light_gizmo(entt::registry& registry) -> void {
    auto cube = entt::locator<MeshCache>::value()["cube"_hs];
    auto shader = entt::locator<ShaderCache>::value()["light"_hs];
    auto const& renderer_context = entt::locator<RendererContext>::value();

    auto lights = registry.view<BaseComponent, LightComponent>();

    // TODO: refactor how to access camera
    auto [entity, camera_base, camera] = *registry.view<BaseComponent, CameraComponent>().each().begin();

    for (auto [entity, base, light] : lights.each()) {
        if (!base.enabled) {
            continue;
        }

        if (!light.draw_gizmo) {
            continue;
        }

        Transform transform = base.world_transform(registry);
        transform.scale = vec3(0.2);
        transform.rotation = glm::identity<glm::quat>();

        shader->set_uniform("model", transform.matrix());
        shader->set_uniform("view", camera.view_matrix(camera_base));
        shader->set_uniform("projection", renderer_context.projection);
        shader->set_uniform("color", light.ambient);

        cube->draw();
    }
}