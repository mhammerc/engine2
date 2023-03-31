#include "../../components/base_component.h"
#include "../../components/material_component.h"
#include "../../components/outline_component.h"
#include "../shader_cache.h"
#include "deferred_renderer.h"

using namespace engine;

static auto draw_entity(
    entt::registry const& registry,
    ShaderProgram* shader,
    entt::entity entity,
    BaseComponent const& base,
    MaterialComponent const& material
) -> void {
    auto model = base.world_matrix(registry);
    auto model_normal = mat3(glm::transpose(glm::inverse(model)));

    shader->set_uniform("model", model);
    shader->set_uniform("model_normal", model_normal);

    // +1 because entity_id 0 will be entt::null.
    auto const entity_id = static_cast<u16>(entity) + 1;
    shader->set_uniform("identity", entity_id);

    shader->set_uniform("is_outline", registry.all_of<OutlineComponent>(entity));

    if (material.diffuse) {
        material.diffuse->activate_as(1);
        shader->set_uniform("texture_diffuse1", 1);
    }

    if (material.specular) {
        material.specular->activate_as(2);
        shader->set_uniform("texture_specular1", 2);
    }

    if (material.normal) {
        material.normal->activate_as(3);
        shader->set_uniform("texture_normal1", 3);
    }

    material.mesh->draw();
}

/**
 * This first pass is quite simple. We bind our gbuffers (see deferred_renderer.h for more information) and draw into
 * them each drawable entity. A drawable entity is enabled and have the following
 * components: BaseComponent, MaterialComponent.
 *
 * We do not draw the skybox, gizmos, etc. Just plain old drawable entities.
 *
 * You can take a look at the shader, it's simple.
 */
auto DeferredRenderer::pass_geometry(entt::registry const& registry, RendererContext renderer_context) -> void {
    auto shader = entt::locator<ShaderCache>::value()["dr_pass_gbuffers"_hs];

    shader->set_uniform("projection", renderer_context.projection);
    shader->set_uniform("view", renderer_context.view);

    _gbuffers->bind();
    shader->bind();

    if (renderer_context.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto drawable_entities = registry.view<BaseComponent, MaterialComponent>();
    for (auto const& [entity, base, material] : drawable_entities.each()) {
        if (!base.enabled) {
            continue;
        }

        draw_entity(registry, shader.handle().get(), entity, base, material);
    }

    if (renderer_context.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    shader->unbind();
    _gbuffers->unbind();
}