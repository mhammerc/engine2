#include "deferred_renderer.h"

#include "../../common.h"
#include "../../components/outline_component.h"

using namespace engine;

auto DeferredRenderer::create(std::string const& name) -> std::unique_ptr<DeferredRenderer> {
    auto renderer = std::make_unique<DeferredRenderer>(DeferredRenderer());
    renderer->_name = name;

    // Create _gbuffers
    {
        using Attachment = Framebuffer::AttachmentDescription;

        std::array<Framebuffer::AttachmentDescription, 5> const attachments = {{
            // Albedo (RGB)
            {
                Attachment::Format::RGB,
                Attachment::Type::Texture2D,
                Attachment::TargetBuffer::Color0,
                "Albedo (RGB)",
            },
            // Normal World (XYZ)
            {
                Attachment::Format::RGB16SNORM,
                Attachment::Type::Texture2D,
                Attachment::TargetBuffer::Color1,
                "Normal World (XYZ)",
            },
            // Specular (R), Is To-Be-Outlined (G), Unused (B)
            {
                Attachment::Format::RGB,
                Attachment::Type::Texture2D,
                Attachment::TargetBuffer::Color2,
                "Specular (R), To-Be-Outlined (G), Unused (B)",
            },
            // Identify (X), Unused (GBA)
            {
                Attachment::Format::RGBA16F,
                Attachment::Type::Texture2D,
                Attachment::TargetBuffer::Color3,
                "Identify (X), Unused (GBA)",
            },
            // Depth
            {
                Attachment::Format::Depth,
                Attachment::Type::Texture2D,
                Attachment::TargetBuffer::Depth,
                "Depth",
            },
        }};

        auto framebuffer_name = fmt::format("{}_gbuffers", name);
        auto framebuffer = Framebuffer::create_with_attachments(framebuffer_name, {1, 1}, attachments);

        renderer->_gbuffers = std::move(framebuffer);
    }

    auto before_post_processing_name = fmt::format("{}_before_post_processing", name);
    renderer->_before_post_processing = Framebuffer::create_with_color_and_depth(before_post_processing_name, {1, 1});

    auto after_post_processing_name = fmt::format("{}_after_post_processing", name);
    renderer->_after_post_processing = Framebuffer::create_with_color(after_post_processing_name, {1, 1});

    return renderer;
};

auto DeferredRenderer::copy_gbuffer_depth_to_before_post_processing() -> void {
    PROFILER_BLOCK("Copy G-Buffer depth to _before_post_processing");

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _gbuffers->handle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _before_post_processing->handle());

    glBlitFramebuffer(
        0,
        0,
        _gbuffers->size().x,
        _gbuffers->size().y,
        0,
        0,
        _before_post_processing->size().x,
        _before_post_processing->size().y,
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
    );

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

auto DeferredRenderer::copy_color_and_outline_to_destination(DrawDestination destination) -> void {
    PROFILER_BLOCK("Copy to Destination");

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _after_post_processing->handle());
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.framebuffer->handle());
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBlitFramebuffer(
        0,
        0,
        _before_post_processing->size().x,
        _before_post_processing->size().y,
        destination.position.x,
        destination.position.y,
        destination.position.x + destination.size.x,
        destination.position.y + destination.size.y,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _gbuffers->handle());
    glReadBuffer(GL_COLOR_ATTACHMENT3);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.framebuffer->handle());
    glDrawBuffer(GL_COLOR_ATTACHMENT1);

    glBlitFramebuffer(
        0,
        0,
        _before_post_processing->size().x,
        _before_post_processing->size().y,
        destination.position.x,
        destination.position.y,
        destination.position.x + destination.size.x,
        destination.position.y + destination.size.y,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

auto DeferredRenderer::draw(
    entt::registry const& registry,
    DrawDestination draw_destination,
    RendererContext renderer_context
) -> void {
    if (draw_destination.size.x <= 0 || draw_destination.size.y <= 0) {
        // Nothing to render
        return;
    }

    PROFILER_BLOCK("DeferredRenderer::draw ({})", _name);

    {
        PROFILER_BLOCK("Resize and clear");
        _gbuffers->resize(draw_destination.size);
        _before_post_processing->resize(draw_destination.size);
        _after_post_processing->resize(draw_destination.size);
        _gbuffers->clear();
    }

    pass_geometry(registry, renderer_context);
    // pass_shadow(registry, renderer_context);
    pass_opaque(registry, renderer_context);
    copy_gbuffer_depth_to_before_post_processing();
    // pass_transparent(registry, renderer_context);
    pass_skybox(registry, renderer_context);
    pass_gizmo(registry, renderer_context);
    pass_bloom(renderer_context);
    pass_postprocess(renderer_context);
    pass_outline();

    copy_color_and_outline_to_destination(draw_destination);
}

auto DeferredRenderer::size() const -> vec2i {
    return _gbuffers->size();
}

auto DeferredRenderer::name() -> std::string& {
    return _name;
}

auto DeferredRenderer::gbuffers() const -> Framebuffer* {
    return _gbuffers.get();
}

auto DeferredRenderer::before_post_processing() const -> Framebuffer* {
    return _before_post_processing.get();
}

auto DeferredRenderer::after_post_processing() const -> Framebuffer* {
    return _after_post_processing.get();
}

auto DeferredRenderer::downsamples() const -> std::span<const std::unique_ptr<Framebuffer>> {
    return _downsamples;
}

auto DeferredRenderer::upsamples() const -> std::span<const std::unique_ptr<Framebuffer>> {
    return _upsamples;
}

// TODO: refactor the following : we also need the attachment id (0,1,2,...)
auto DeferredRenderer::albedo_texture() const -> Texture* {
    return _gbuffers->attachments()[0].second.get();
}

auto DeferredRenderer::normal_texture() const -> Texture* {
    return _gbuffers->attachments()[1].second.get();
}

auto DeferredRenderer::specular_texture() const -> Texture* {
    return _gbuffers->attachments()[2].second.get();
}

auto DeferredRenderer::identify_texture() const -> Texture* {
    return _gbuffers->attachments()[3].second.get();
}

auto DeferredRenderer::depth_texture() const -> Texture* {
    return _gbuffers->attachments()[4].second.get();
}