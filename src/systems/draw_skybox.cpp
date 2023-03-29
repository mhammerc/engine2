#include "../components/base_component.h"
#include "../components/skybox_component.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/shader_cache.h"
#include "../graphics/texture/texture_cache.h"
#include "systems.h"

using namespace engine;

auto systems::draw_skybox(entt::registry& registry, RendererContext renderer_context) -> void {
    glDepthFunc(GL_LEQUAL);

    auto skyboxes = registry.view<BaseComponent, SkyboxComponent>();

    auto cubemap = entt::locator<TextureCache>::value()["skybox"_hs];
    auto shader = entt::locator<ShaderCache>::value()["skybox"_hs];
    auto cube = entt::locator<MeshCache>::value()["cube"_hs];

    cubemap->activate_as(0);
    shader->set_uniform("skybox", 0);

    // Remove translation from the view matrix.
    // For the skybox, only rotation is needed.
    auto view = glm::mat4(glm::mat3(renderer_context.view));
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
