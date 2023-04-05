#include "../mesh_cache.h"
#include "../shader_cache.h"
#include "deferred_renderer.h"

using namespace engine;

// static auto post_process_uniform_value(RendererContext renderer_context) -> int {
//     int post_process = 0;
//     if (renderer_context.post_process.inverse) {
//         post_process |= RendererContext::PostProcessing::POST_PROCESS_INVERSE;
//     }
//     if (renderer_context.post_process.black_and_white) {
//         post_process |= RendererContext::PostProcessing::POST_PROCESS_GRAYSCALE;
//     }
//     if (renderer_context.post_process.sepia) {
//         post_process |= RendererContext::PostProcessing::POST_PROCESS_SEPIA;
//     }
//     if (renderer_context.post_process.blur) {
//         post_process |= RendererContext::PostProcessing::POST_PROCESS_BLUR;
//     }
//     if (renderer_context.post_process.sharpen) {
//         post_process |= RendererContext::PostProcessing::POST_PROCESS_SHARPEN;
//     }
//     if (renderer_context.post_process.edge_dectection) {
//         post_process |= RendererContext::PostProcessing::POST_PROCESS_EDGE_DETECTION;
//     }

//     return post_process;
// }

auto DeferredRenderer::pass_postprocess(RendererContext renderer_context) -> void {
    auto& mesh_cache = entt::locator<MeshCache>::value();
    auto& shader_cache = entt::locator<ShaderCache>::value();

    auto mesh_quad = mesh_cache["quad"_hs];
    auto postprocess_shader = shader_cache["postprocess"_hs];

    _before_post_processing->color()->activate_as(0);
    postprocess_shader->set_uniform("screen_texture", 0);

    postprocess_shader->set_uniform("exposure", renderer_context.exposure);
    postprocess_shader->set_uniform("tone_mapping", static_cast<i32>(renderer_context.tone_mapping));

    postprocess_shader->set_uniform("bloom_enabled", renderer_context.bloom_enabled);
    postprocess_shader->set_uniform("bloom_intensity", renderer_context.bloom_intensity);

    if (renderer_context.bloom_enabled) {
        _upsamples[0]->color()->activate_as(1);
        postprocess_shader->set_uniform("bloom_texture", 1);
    }

    glDisable(GL_DEPTH_TEST);
    _after_post_processing->bind();
    postprocess_shader->bind();

    mesh_quad->draw();

    postprocess_shader->unbind();
    _after_post_processing->unbind();
    glEnable(GL_DEPTH_TEST);

    _before_post_processing->color()->activate_as(0, true);
}
