#include "../../components/base_component.h"
#include "../../components/skybox_component.h"
#include "../mesh_cache.h"
#include "../shader_cache.h"
#include "deferred_renderer.h"

using namespace engine;

auto DeferredRenderer::pass_skybox(entt::registry const& registry, RendererContext renderer_context) -> void {
    PROFILER_BLOCK("Skybox Pass");

    auto skyboxes = registry.view<BaseComponent, SkyboxComponent>();

    auto shader = entt::locator<ShaderCache>::value()["skybox"_hs];
    auto cube = entt::locator<MeshCache>::value()["cube"_hs];

    // The skybox requires only rotation, not any translation.
    // Because the skybox looks infinitely far from the player,
    // it is easier to render a 1x1x1 cube around the player with
    // the proper depth testing configuration.
    // Reminder: the translation part of a mat4 matrix is the last column. Thus,
    // we keep only the first three columns by casting it to a mat3.
    auto view = glm::mat4(glm::mat3(renderer_context.view));
    shader->set_uniform("view", view);
    shader->set_uniform("projection", renderer_context.projection);

    _before_post_processing->bind();
    shader->bind();
    glCullFace(GL_FRONT);  // Invert culling to draw the inside of the mesh cube.
    glDepthFunc(GL_EQUAL);  // Draw the skybox only at max distance depth. The shader output vertices at max depth.

    for (auto [entity, base, skybox] : skyboxes.each()) {
        if (!base.enabled) {
            continue;
        }

        skybox.cubemap->activate_as(0);
        shader->set_uniform("skybox", 0);

        cube->draw();

        skybox.cubemap->activate_as(0, true);

        // We break because there is no sense to draw more than 1 skybox.
        break;
    }

    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    shader->unbind();
    _before_post_processing->unbind();
}