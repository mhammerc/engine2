
#include "../components/outline_component.h"
#include "../graphics/deferred_renderer_cache.h"
#include "../graphics/framebuffer_cache.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/renderer_context.h"
#include "../graphics/shader_cache.h"
#include "systems.h"

using namespace engine;

/**
 * Using the previous intermediate buffer as a texture, draw the outline on the existing color framebuffer.
 */
static auto draw(Framebuffer& framebuffer_color, Texture* texture_outline) -> void {
    auto quad = entt::locator<MeshCache>::value()["quad"_hs];
    auto shader = entt::locator<ShaderCache>::value()["outline"_hs];

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    framebuffer_color.bind();
    texture_outline->activate_as(0);

    shader->set_uniform("outline_texture", 0);

    shader->bind();
    quad->draw();
    shader->unbind();

    texture_outline->activate_as(0, true);
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
auto systems::draw_outline(entt::registry& registry, RendererContext renderer_context) -> void {
    auto& framebuffer_cache = entt::locator<FramebufferCache>::value();
    auto& deferred_render_cache = entt::locator<DeferredRendererCache>::value();

    auto framebuffer_color = framebuffer_cache["postprocess"_hs];
    auto deferred_render = deferred_render_cache["renderer"_hs];

    // TODO: move the draw function to the postprocess shader?
    draw(framebuffer_color, deferred_render->specular_texture());
}
