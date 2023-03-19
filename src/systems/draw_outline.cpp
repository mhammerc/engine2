#include "../graphics/framebuffer_cache.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/renderer_context.h"
#include "../graphics/shader_cache.h"
#include "../scene/components/base_component.h"
#include "../scene/components/camera_component.h"
#include "../scene/components/material_component.h"
#include "../scene/components/outline_component.h"
#include "systems.h"

using namespace engine;

/**
 * From a black framebuffer, draw all to-be-outlined objects as white.
 */
static auto draw_intermediate_buffer(entt::registry& registry, Framebuffer& frame_buffer) -> void {
    auto shader = entt::locator<ShaderCache>::value()["unlit_single_color"_hs];

    auto camera = *registry.view<BaseComponent, CameraComponent>().begin();
    auto const& [camera_base, camera_config] = registry.get<BaseComponent, CameraComponent>(camera);

    frame_buffer.bind();

    glClearColor(0.F, 0.F, 0.F, 1.F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    auto const& renderer_context = entt::locator<RendererContext>::value();

    auto entities = registry.view<BaseComponent, MaterialComponent, OutlineComponent>();
    for (auto const& [entity, base, material] : entities.each()) {
        if (!base.enabled) {
            continue;
        }

        auto const model = base.world_matrix(registry);

        // MVP
        shader->set_uniform("model", model);
        shader->set_uniform("view", camera_config.view_matrix(camera_base));
        shader->set_uniform("projection", renderer_context.projection);
        shader->set_uniform("color", vec4(1.F, 1.F, 1.F, 1.F));

        shader->bind();
        material.mesh->draw();
        shader->unbind();
    }

    frame_buffer.unbind();
}

/**
 * Using the previous intermediate buffer as a texture, draw the outline on the existing color framebuffer.
 */
static auto draw(Framebuffer& framebuffer_color, Framebuffer& framebuffer_outline) -> void {
    auto quad = entt::locator<MeshCache>::value()["quad"_hs];
    auto shader = entt::locator<ShaderCache>::value()["outline"_hs];

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    framebuffer_color.bind();
    framebuffer_outline.color_texture()->activate_as(0);

    shader->set_uniform("outline_texture", 0);

    shader->bind();
    quad->draw();
    shader->unbind();

    framebuffer_outline.color_texture()->activate_as(0, true);
    framebuffer_color.unbind();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

/**
 * This system draw outlines around objects that have `OutlineComponent`.
 *
 * To draw the outline:
 * 1) Bind an intermediate framebuffer.
 * 2) Clear it black.
 * 3) Draw on this buffer all to-be-outlined objects in white.
 *
 * 4) Go back to the color framebuffer
 * 5) Using the texture of the previous black framebuffer, draw the outline. To calculate the outline, check the
 * neighboring pixels.
 */
auto systems::draw_outline(entt::registry& registry) -> void {
    auto& framebuffer_cache = entt::locator<FrameBufferCache>::value();
    auto& [_1, framebuffer_outline] = *framebuffer_cache["outline"_hs];
    auto& [_2, framebuffer_color] = *framebuffer_cache["postprocess"_hs];

    draw_intermediate_buffer(registry, framebuffer_outline);

    draw(framebuffer_color, framebuffer_outline);
}
