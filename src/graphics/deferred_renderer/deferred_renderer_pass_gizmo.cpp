#include "../../components/base_component.h"
#include "../../components/light_component.h"
#include "../mesh_cache.h"
#include "../shader_cache.h"
#include "deferred_renderer.h"

using namespace engine;

/**
 * This pass draw the various scene gizmos. For now we have one gizmo only: lights.
 */
auto DeferredRenderer::pass_gizmo(entt::registry const& registry, RendererContext renderer_context) -> void {
    PROFILER_BLOCK("Gizmos Pass");

    auto cube = entt::locator<MeshCache>::value()["cube"_hs];
    auto shader = entt::locator<ShaderCache>::value()["light"_hs];

    shader->bind();
    _before_post_processing->bind();

    auto lights = registry.view<BaseComponent, LightComponent>();
    for (auto [entity, base, light] : lights.each()) {
        if (!base.enabled) {
            continue;
        }

        if (!light.draw_gizmo) {
            continue;
        }

        Transform transform = base.world_transform(registry);
        transform.scale = vec3(0.2F);
        transform.rotation = glm::identity<glm::quat>();

        shader->set_uniform("model", transform.matrix());
        shader->set_uniform("view", renderer_context.view);
        shader->set_uniform("projection", renderer_context.projection);
        shader->set_uniform("color", light.diffuse);

        cube->draw();
    }

    _before_post_processing->unbind();
    shader->unbind();
}