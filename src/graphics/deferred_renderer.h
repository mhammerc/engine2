#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <string>

#include "framebuffer.h"
#include "renderer_context.h"

namespace engine {

/**
 * DeferredRenderer glue together the required components to perform rendering. As its name implies, it is a deferred rendering.
 */
class DeferredRenderer {
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
    auto draw(DrawDestination draw_destination, entt::registry const& registry, RendererContext renderer_context)
        -> void;

    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto framebuffer() const -> Framebuffer*;
    [[nodiscard]] auto name() -> std::string&;
    [[nodiscard]] auto albedo_texture() const -> Texture*;
    [[nodiscard]] auto normal_texture() const -> Texture*;
    [[nodiscard]] auto specular_texture() const -> Texture*;
    [[nodiscard]] auto identify_texture() const -> Texture*;
    [[nodiscard]] auto depth_texture() const -> Texture*;

  private:
    auto
    pass_geometry(DrawDestination draw_destination, entt::registry const& registry, RendererContext renderer_context)
        -> void;
    auto
    pass_lighting(DrawDestination draw_destination, entt::registry const& registry, RendererContext renderer_context)
        -> void;

  private:
    std::unique_ptr<Framebuffer> _gbuffers;
    std::string _name;
};

}  // namespace engine