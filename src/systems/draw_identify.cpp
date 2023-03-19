#include "../graphics/framebuffer_cache.h"
#include "../graphics/renderer_context.h"
#include "../graphics/shader_cache.h"
#include "../scene/components/base_component.h"
#include "../scene/components/camera_component.h"
#include "../scene/components/material_component.h"
#include "systems.h"

using namespace engine;

auto systems::draw_identify(entt::registry& registry) -> void {
    auto shader = entt::locator<ShaderCache>::value()["unlit_single_color"_hs];
    auto framebuffer = entt::locator<FramebufferCache>::value()["identify"_hs];
    auto const& renderer_context = entt::locator<RendererContext>::value();

    auto camera = *registry.view<BaseComponent, CameraComponent>().begin();
    auto const& [camera_base, camera_config] = registry.get<BaseComponent, CameraComponent>(camera);

    framebuffer->bind();

    glClearColor(0.F, 0.F, 0.2F, 1.F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    auto entities = registry.view<BaseComponent, MaterialComponent>();
    for (auto const& [entity, base, material] : entities.each()) {
        if (!base.enabled) {
            continue;
        }

        auto const model = base.world_matrix(registry);

        auto const entity_id = static_cast<u32>(entity);
        auto const color = vec4(static_cast<float>((entity_id & 0xFF)) / 255.F);

        // MVP
        shader->set_uniform("model", model);
        shader->set_uniform("view", camera_config.view_matrix(camera_base));
        shader->set_uniform("projection", renderer_context.projection);
        shader->set_uniform("color", color);

        shader->bind();
        material.mesh->draw();
        shader->unbind();
    }

    framebuffer->unbind();
}
