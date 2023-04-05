#pragma once

#include <memory>
#include <string>

#include "../../core/noncopyable.h"
#include "../framebuffer.h"
#include "../renderer_context.h"
#include "entt/entt.hpp"

namespace engine {

/**
 * DeferredRenderer perform the multiple rendering passes to draw a complete scene.
 *
 * Overview of render passes:
 * 1) Geometry pass: render the geometry buffers (g-buffers) (albedo, specular, world normals and more)
 * 2) Shadow pass: render the shadowmaps
 * 3) Opaque pass: render opaque objects using the g-buffers.
 * 4) TODO: transparent pass
 * 5) Skybox pass: a scene can have maximum one skybox at a time. If there is one skybox, draw it.
 * 6) Gizmos pass: draw the scene gizmos. (eg. lights)
 * 7) Bloom pass: perform the bloom pass using
 * 8) Postprocess pass: perform the tone mapping, HDR scaling, and more.
 * 9) Outline pass: draw a nice outline around objects that were selected to be outlined in gbuffers.
 *
 * 10) We then copy two texture to the destination framebuffer:
 *   a) COLOR0: the complete drawn image (RGB)
 *   b) COLOR1: Identify data (RGBA16F): Identify (R), Unused (GBA)
 *
 * To render a complete scene, just call the draw function.
 */
class DeferredRenderer: public noncopyable {
  public:
    static auto create(std::string const& name) -> std::unique_ptr<DeferredRenderer>;

  public:
    struct DrawDestination {
        Framebuffer* framebuffer;
        vec2i position;
        vec2i size;
    };

    /**
     * Draw the scene to a specific framebuffer at a given position and size.
     *
     * It will resize the buffers to the proper size.
     */
    auto draw(entt::registry const& registry, DrawDestination draw_destination, RendererContext renderer_context)
        -> void;

    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto name() -> std::string&;

    [[nodiscard]] auto gbuffers() const -> Framebuffer*;
    [[nodiscard]] auto before_post_processing() const -> Framebuffer*;
    [[nodiscard]] auto after_post_processing() const -> Framebuffer*;
    [[nodiscard]] auto downsamples() const -> std::span<const std::unique_ptr<Framebuffer>>;
    [[nodiscard]] auto upsamples() const -> std::span<const std::unique_ptr<Framebuffer>>;

    [[nodiscard]] auto albedo_texture() const -> Texture*;
    [[nodiscard]] auto normal_texture() const -> Texture*;
    [[nodiscard]] auto specular_texture() const -> Texture*;
    [[nodiscard]] auto identify_texture() const -> Texture*;
    [[nodiscard]] auto depth_texture() const -> Texture*;

  private:
    auto pass_geometry(entt::registry const& registry, RendererContext renderer_context) -> void;
    auto pass_opaque(entt::registry const& registry, RendererContext renderer_context) -> void;
    auto pass_skybox(entt::registry const& registry, RendererContext renderer_context) -> void;
    auto pass_gizmo(entt::registry const& registry, RendererContext renderer_context) -> void;
    auto pass_bloom(RendererContext renderer_context) -> void;
    auto pass_postprocess(RendererContext renderer_context) -> void;
    auto pass_outline() -> void;

    auto copy_gbuffer_depth_to_before_post_processing() -> void;
    auto copy_color_and_outline_to_destination(DrawDestination destination) -> void;

  private:
    std::unique_ptr<Framebuffer> _gbuffers;
    std::unique_ptr<Framebuffer> _before_post_processing;
    std::unique_ptr<Framebuffer> _after_post_processing;

    /**
     * For bloom processing.
     *
     * Each element is half the size of the previous.
     *
     * [0] = half the size of _before_post_processing
     * [1] = half the size of [0]
     * ...
     */
    std::vector<std::unique_ptr<Framebuffer>> _downsamples;
    std::vector<std::unique_ptr<Framebuffer>> _upsamples;

    std::string _name;
};

}  // namespace engine