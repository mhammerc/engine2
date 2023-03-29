#include "../components/base_component.h"
#include "../components/camera_component.h"
#include "../components/light_component.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/shader_cache.h"
#include "glm/ext/matrix_transform.hpp"
#include "systems.h"

using namespace engine;

auto systems::draw_light_gizmo(entt::registry& registry, RendererContext renderer_context) -> void {
    auto cube = entt::locator<MeshCache>::value()["cube"_hs];
    auto shader = entt::locator<ShaderCache>::value()["light"_hs];

    auto lights = registry.view<BaseComponent, LightComponent>();

    // TODO: refactor how to access camera
    auto [entity, camera_base, camera] = *registry.view<BaseComponent, CameraComponent>().each().begin();

    shader->bind();

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

    shader->unbind();
}