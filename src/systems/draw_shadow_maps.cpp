#include <spdlog/fmt/fmt.h>

#include <array>

#include "../common.h"
#include "../graphics/framebuffer_cache.h"
#include "../graphics/shader_cache.h"
#include "../scene/components/base_component.h"
#include "../scene/components/light_component.h"
#include "../scene/components/material_component.h"
#include "systems.h"

using namespace engine;

constexpr vec2i SHADOW_SIZE = {1024, 1024};

static auto get_framebuffer() -> Framebuffer* {
    auto& framebuffer_cache = entt::locator<FramebufferCache>::value();

    auto framebuffer_insertion = framebuffer_cache.load("shadow_fb"_hs, []() {
        return Framebuffer::create(
            "shadow_fb",
            SHADOW_SIZE,
            Framebuffer::Content::DepthStencil,
            Framebuffer::Type::CubeMap,
            Framebuffer::DepthFormat::Depth
        );
    });

    auto framebuffer_resource = framebuffer_insertion.first->second;
    auto* framebuffer = framebuffer_resource.handle().get();

    return framebuffer;
}

static auto draw_point_light_shadows(entt::registry& registry) -> void {
    auto [light, light_base, light_info] = *registry.view<BaseComponent, LightComponent>().each().begin();
    auto light_position = light_base.world_transform(registry).position;
    auto shader = entt::locator<ShaderCache>::value()["light_point_shadow_map"_hs];

    auto* framebuffer = get_framebuffer();
    framebuffer->bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 shadow_projection;

    {
        float const aspect = static_cast<float>(SHADOW_SIZE.x) / static_cast<float>(SHADOW_SIZE.y);
        float const near = 1.0F;
        float const far = 25.0F;
        shadow_projection = glm::perspective(glm::radians(90.F), aspect, near, far);
    }

    std::array<glm::mat4, 6> shadow_transforms {};
    shadow_transforms[0] = shadow_projection
        * glm::lookAt(light_position, light_position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    shadow_transforms[1] = shadow_projection
        * glm::lookAt(light_position, light_position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    shadow_transforms[2] = shadow_projection
        * glm::lookAt(light_position, light_position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    shadow_transforms[3] = shadow_projection
        * glm::lookAt(light_position, light_position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
    shadow_transforms[4] = shadow_projection
        * glm::lookAt(light_position, light_position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
    shadow_transforms[5] = shadow_projection
        * glm::lookAt(light_position, light_position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

    for (int i = 0; i < 6; ++i) {
        shader->set_uniform(fmt::format("shadow_matrices[{}]", i), shadow_transforms[i]);
    }
    shader->set_uniform("light_pos", light_position);
    shader->set_uniform("far_plane", 25.F);

    shader->bind();
    for (auto [entity, base, material] : registry.view<BaseComponent, MaterialComponent>().each()) {
        auto model = base.world_matrix(registry);
        shader->set_uniform("model", model);

        material.mesh->draw();
    }
    shader->unbind();

    framebuffer->unbind();
}

auto systems::draw_shadow_maps(entt::registry& registry) -> void {
    draw_point_light_shadows(registry);
}