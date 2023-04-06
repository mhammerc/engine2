#include <fmt/core.h>

#include <entt/entt.hpp>

#include "../mesh_cache.h"
#include "../shader_cache.h"
#include "deferred_renderer.h"
#include "entt/core/hashed_string.hpp"

using namespace engine;

/**
 * The bloom rendering pass comes from “Froyok” Léna Piquet:
 * https://www.froyok.fr/blog/2021-12-ue4-custom-bloom/
 *
 * 1) Downsample the scene multiple times until it reach a width of ~8 pixels. Each downsample is half the size.
 *    The downsampling algorithm is a custom bilinear filtering. See the downsampling shader.
 *
 * 2) Then we upsample back to the full scene size following this algorithm:
 *    a) We have downsamples A, B, C, D, etc. Each is half the size of the previous.
 *    b) C' is the first upsample.
 *    c) C' = mix(C, upsample(D))
 *    d) B' = mix(B, upsample(C'))
 *       A' = mix(A, upsample(B'))
 *       etc
 *
 *   There is a custom upsample algorithm you can find in the shader.
 *
 * 3) Mix the latest upsample to the color scene before the tone mapping.
 *    hdr_color = mix(scene, latest_upsample)
 *
 */

static auto downsample_by_half(Framebuffer* to, Framebuffer* from) -> void {
    PROFILER_BLOCK("downsample_{}x{}", to->size().x, to->size().y);

    auto shader = entt::locator<ShaderCache>::value()["dr_pass_bloom_downsample_by_half"_hs];
    auto quad = entt::locator<MeshCache>::value()["quad"_hs];

    from->color()->activate_as(0);
    shader->set_uniform("tex_to_downsample", 0);
    shader->set_uniform("tex_to_downsample_size", static_cast<vec2>(to->size()));

    glDisable(GL_DEPTH_TEST);
    to->bind();
    shader->bind();

    quad->draw();

    shader->unbind();
    to->unbind();
    glEnable(GL_DEPTH_TEST);

    from->color()->activate_as(0, true);
}

static auto upsample_and_combine_by_half(Framebuffer* to, Framebuffer* current, Framebuffer* downsampled, float radius)
    -> void {
    PROFILER_BLOCK("upsample_{}x{}", to->size().x, to->size().y);

    auto shader = entt::locator<ShaderCache>::value()["dr_pass_bloom_upsample_by_half"_hs];
    auto quad = entt::locator<MeshCache>::value()["quad"_hs];

    downsampled->color()->activate_as(0);
    shader->set_uniform("tex_to_upsample", 0);

    current->color()->activate_as(1);
    shader->set_uniform("tex_current", 1);
    shader->set_uniform("tex_current_size", static_cast<vec2>(current->size()));

    shader->set_uniform("radius", radius);

    glDisable(GL_DEPTH_TEST);
    to->bind();
    shader->bind();

    quad->draw();

    shader->unbind();
    to->unbind();
    glEnable(GL_DEPTH_TEST);

    current->color()->activate_as(1, true);
    downsampled->color()->activate_as(0, true);
}

auto DeferredRenderer::pass_bloom(RendererContext renderer_context) -> void {
    if (!renderer_context.bloom_enabled) {
        return;
    }

    PROFILER_BLOCK("Bloom Pass");

    // We want to downsample our scene up to a width of ~8 pixels.
    // We downsample by half for each pass.
    // How much downsample pass to perform?
    u32 number_of_downsample = 0;
    auto* start_texture = _before_post_processing->color();
    int start_width = _before_post_processing->color()->size().x;
    for (int width = start_width; width > 8; width /= 2) {
        number_of_downsample += 1;
    }

    // The upsample algorithm use two downsampled buffers to upsample one time.
    // We will then upsample one less time than we downsample.
    u32 number_of_upsample = number_of_downsample - 1;

    // We know that we want to perform the downsampling `number_of_downsamples` times.
    // The width of the last downsample is `start_width / 2^number_of_downsamples`.
    // We can now prepare the framebuffer array.
    for (u32 i = 0; i < number_of_downsample; ++i) {
        auto size = start_texture->size() / static_cast<int>(std::pow(2, i + 1));

        if (i < _downsamples.size()) {
            // The framebuffer already exist. Resize it to the proper size.
            _downsamples[i]->resize(size);
        } else {
            // The framebuffer doesn't exist. It happens on the first render or if the resolution change and we have
            // more downsamples to compute.
            // Create the framebuffer.
            auto name = fmt::format("Downsample Pass {}", i);

            auto attachment = Framebuffer::AttachmentDescription {
                .format = Framebuffer::AttachmentDescription::Format::RGBA16F,
                .type = Framebuffer::AttachmentDescription::Type::Texture2D,
                .target_buffer = Framebuffer::AttachmentDescription::TargetBuffer::Color0,
                .name = name,
            };

            auto framebuffer = Framebuffer::create_with_attachments(name, size, singular_span(attachment));

            _downsamples.emplace_back(std::move(framebuffer));
        }
    }

    // Same for the upsample array.
    for (u32 i = 0; i < number_of_upsample; ++i) {
        auto size = start_texture->size() / static_cast<int>(std::pow(2, i + 1));

        if (i < _upsamples.size()) {
            _upsamples[i]->resize(size);
        } else {
            auto name = fmt::format("Upsample Pass {}", i);

            auto attachment = Framebuffer::AttachmentDescription {
                .format = Framebuffer::AttachmentDescription::Format::RGBA16F,
                .type = Framebuffer::AttachmentDescription::Type::Texture2D,
                .target_buffer = Framebuffer::AttachmentDescription::TargetBuffer::Color0,
                .name = name,
            };

            auto framebuffer = Framebuffer::create_with_attachments(name, size, singular_span(attachment));

            _upsamples.emplace_back(std::move(framebuffer));
        }
    }

    // Delete the unused framebuffers.
    // This step is optional but it allows the following code to be easier and easier debugging in the texture viewer.
    if (number_of_downsample < _downsamples.size()) {
        _downsamples.resize(number_of_downsample);
        _upsamples.resize(number_of_upsample);
    }

    // Downsample...
    for (u32 i = 0; i < number_of_downsample; ++i) {
        Framebuffer* to = _downsamples[i].get();

        Framebuffer* from = nullptr;
        if (i == 0) {
            // Downsample from the color buffer on the first iteration...
            from = _before_post_processing.get();
        } else {
            // ... or from the previous downsample
            from = _downsamples[i - 1].get();
        }

        downsample_by_half(to, from);
    }

    // ...and upsample
    for (u32 i = number_of_upsample - 1; i != UINT_MAX; --i) {
        Framebuffer* to = _upsamples[i].get();
        Framebuffer* downsampled = _downsamples[i].get();
        Framebuffer* current = nullptr;

        if (i == number_of_upsample - 1) {
            current = _downsamples[i + 1].get();
        } else {
            current = _upsamples[i + 1].get();
        }

        upsample_and_combine_by_half(to, current, downsampled, renderer_context.bloom_radius);
    }
}