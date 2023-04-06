#include "../framebuffer_cache.h"
#include "../mesh_cache.h"
#include "../shader_cache.h"
#include "deferred_renderer.h"

using namespace engine;

auto DeferredRenderer::pass_outline() -> void {
    PROFILER_BLOCK("Outline Pass");

    auto& framebuffer_cache = entt::locator<FramebufferCache>::value();
    auto framebuffer_color = framebuffer_cache["postprocess"_hs];

    auto quad = entt::locator<MeshCache>::value()["quad"_hs];
    auto shader = entt::locator<ShaderCache>::value()["outline"_hs];

    glDisable(GL_DEPTH_TEST);  // We do not have any depth information on the target framebuffer.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    specular_texture()->activate_as(0);
    shader->set_uniform("outline_texture", 0);

    _after_post_processing->bind();
    shader->bind();

    quad->draw();

    shader->unbind();
    _after_post_processing->unbind();

    specular_texture()->activate_as(0, true);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}