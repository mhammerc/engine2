#include "../../components/base_component.h"
#include "../../components/light_component.h"
#include "../framebuffer_cache.h"
#include "../mesh_cache.h"
#include "../shader_cache.h"
#include "deferred_renderer.h"

using namespace engine;

/**
 * When this pass runs, the geometry buffers are full of data and ready.
 */
auto DeferredRenderer::pass_opaque(entt::registry const& registry, RendererContext renderer_context) -> void {
    auto quad = entt::locator<MeshCache>::value()["quad"_hs];
    auto shader = entt::locator<ShaderCache>::value()["dr_pass_lighting"_hs];

    albedo_texture()->activate_as(0, false);
    shader->set_uniform("g_albedo", 0);

    normal_texture()->activate_as(1, false);
    shader->set_uniform("g_normal_world", 1);

    specular_texture()->activate_as(2, false);
    shader->set_uniform("g_specular", 2);

    depth_texture()->activate_as(3, false);
    shader->set_uniform("g_depth", 3);

    shader->set_uniform("camera_position", renderer_context.camera_transform.position);

    shader->set_uniform("projection_matrix_inverse", glm::inverse(renderer_context.projection));
    shader->set_uniform("view_matrix_inverse", glm::inverse(renderer_context.view));

    auto* shadow_map = entt::locator<FramebufferCache>::value()["shadow_fb"_hs]->depth();
    shadow_map->activate_as(20);
    shader->set_uniform("shadow_map", 20);
    shader->set_uniform("shadow_map_far_plane", 25.F);

    int light_index = 0;
    auto lights = registry.view<BaseComponent, LightComponent>();
    for (auto [entity, base, light] : lights.each()) {
        if (!base.enabled) {
            continue;
        }

        shader->set_uniform(fmt::format("lights[{}].type", light_index), static_cast<int>(light.type));
        shader->set_uniform(fmt::format("lights[{}].position", light_index), base.world_transform(registry).position);
        shader->set_uniform(
            fmt::format("lights[{}].direction", light_index),
            base.world_transform(registry).direction()
        );
        shader->set_uniform(fmt::format("lights[{}].innerCutOff", light_index), light.inner_cut_off);
        shader->set_uniform(fmt::format("lights[{}].outerCutOff", light_index), light.outer_cut_off);
        shader->set_uniform(fmt::format("lights[{}].constant", light_index), light.constant);
        shader->set_uniform(fmt::format("lights[{}].linear", light_index), light.linear);
        shader->set_uniform(fmt::format("lights[{}].quadratic", light_index), light.quadratic);
        shader->set_uniform(fmt::format("lights[{}].ambient", light_index), light.ambient);
        shader->set_uniform(fmt::format("lights[{}].diffuse", light_index), light.diffuse);
        shader->set_uniform(fmt::format("lights[{}].specular", light_index), light.specular);
        light_index += 1;
    }
    while (light_index < 10) {
        shader->set_uniform(fmt::format("lights[{}].type", light_index), static_cast<int>(LightComponent::Unset));
        ++light_index;
    }

    _before_post_processing->bind();
    shader->bind();

    quad->draw();

    shader->unbind();
    _before_post_processing->unbind();

    shadow_map->activate_as(20, true);
    depth_texture()->activate_as(3, true);
    specular_texture()->activate_as(2, true);
    normal_texture()->activate_as(1, true);
    albedo_texture()->activate_as(0, true);
}