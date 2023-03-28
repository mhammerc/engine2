#include "deferred_renderer.h"

#include <spdlog/fmt/fmt.h>

#include "../common.h"
#include "../scene/components/base_component.h"
#include "../scene/components/light_component.h"
#include "../scene/components/material_component.h"
#include "framebuffer_cache.h"
#include "mesh_cache.h"
#include "shader_cache.h"

using namespace engine;

auto DeferredRenderer::create(std::string const& name) -> std::unique_ptr<DeferredRenderer> {
    using Attachment = Framebuffer::AttachmentDescription;

    std::array<Framebuffer::AttachmentDescription, 4> const attachments = {{
        // Albedo (RGB)
        {
            Attachment::Format::RGB,
            Attachment::Type::Texture2D,
            Attachment::TargetBuffer::Color0,
        },
        // Normal World (XYZ)
        {
            Attachment::Format::RGB16SNORM,
            Attachment::Type::Texture2D,
            Attachment::TargetBuffer::Color1,
        },
        // Specular (R)
        {
            Attachment::Format::RGB,
            Attachment::Type::Texture2D,
            Attachment::TargetBuffer::Color2,
        },
        // Depth
        {
            Attachment::Format::Depth,
            Attachment::Type::Texture2D,
            Attachment::TargetBuffer::Depth,
        },
    }};

    auto framebuffer_name = fmt::format("{}_gbuffers", name);
    auto framebuffer = Framebuffer::create_with_attachments(framebuffer_name, {1, 1}, attachments);

    auto renderer = std::make_unique<DeferredRenderer>(DeferredRenderer());
    renderer->_gbuffers = std::move(framebuffer);
    renderer->_name = name;

    return renderer;
};

auto DeferredRenderer::draw(
    DrawDestination draw_destination,
    entt::registry const& registry,
    mat4 projection,
    mat4 view,
    vec3 camera_position
) -> void {
    pass_geometry(registry, draw_destination, projection, view);
    pass_lighting(draw_destination, registry, projection, view, camera_position);

    // Copy the proper depth buffer to the destination framebuffer
    auto size = draw_destination.size;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _gbuffers->handle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_destination.framebuffer->handle());  // write to default framebuffer
    glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

auto DeferredRenderer::pass_geometry(
    entt::registry const& registry,
    DrawDestination destination,
    mat4 projection,
    mat4 view
) -> void {
    _gbuffers->resize(destination.size);

    auto shader = entt::locator<ShaderCache>::value()["dr_pass_gbuffers"_hs];

    shader->set_uniform("projection", projection);
    shader->set_uniform("view", view);

    _gbuffers->bind();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto const& [entity, base, material] : registry.view<BaseComponent, MaterialComponent>().each()) {
        if (!base.enabled) {
            return;
        }

        auto model = base.world_matrix(registry);
        auto model_normal = mat3(glm::transpose(glm::inverse(model)));

        shader->set_uniform("model", model);
        shader->set_uniform("model_normal", model_normal);

        // TODO: material textures must be rethink, this whole block
        {
            if (material.textures.contains(1)) {
                shader->set_uniform("texture_diffuse1", 1);
            } else {
                shader->set_uniform("texture_diffuse1", 0);
            }

            if (material.textures.contains(2)) {
                shader->set_uniform("texture_specular1", 2);
            } else {
                shader->set_uniform("texture_specular1", 0);
            }

            shader->bind();
            for (auto const& texture : material.textures) {
                texture.second->activate_as(texture.first, false);
            }
        }

        material.mesh->draw();
        shader->unbind();
    }

    _gbuffers->unbind();
}

auto DeferredRenderer::pass_lighting(
    DrawDestination draw_destination,
    entt::registry const& registry,
    mat4 projection,
    mat4 view,
    vec3 camera_position
) -> void {
    // At this point, we have all the geometry buffers in _gbuffers. Go ahead and render the lighting pass into a quad
    // in draw_destination.framebuffer.
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

    shader->set_uniform("camera_position", camera_position);

    shader->set_uniform("projection_matrix_inverse", glm::inverse(projection));
    shader->set_uniform("view_matrix_inverse", glm::inverse(view));

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

    draw_destination.framebuffer->bind();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->bind();

    quad->draw();

    shader->unbind();
    draw_destination.framebuffer->unbind();

    shadow_map->activate_as(20, true);
    depth_texture()->activate_as(3, true);
    specular_texture()->activate_as(2, true);
    normal_texture()->activate_as(1, true);
    albedo_texture()->activate_as(0, true);
}

auto DeferredRenderer::name() -> std::string& {
    return _name;
}

auto DeferredRenderer::framebuffer() const -> Framebuffer* {
    return _gbuffers.get();
}

auto DeferredRenderer::albedo_texture() const -> Texture* {
    return _gbuffers->attachments()[0].second.get();
}

auto DeferredRenderer::normal_texture() const -> Texture* {
    return _gbuffers->attachments()[1].second.get();
}

auto DeferredRenderer::specular_texture() const -> Texture* {
    return _gbuffers->attachments()[2].second.get();
}

auto DeferredRenderer::depth_texture() const -> Texture* {
    return _gbuffers->attachments()[3].second.get();
}