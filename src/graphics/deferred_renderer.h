#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <string>

#include "framebuffer.h"

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
    auto draw(
        DrawDestination draw_destination,
        entt::registry const& registry,
        mat4 projection,
        mat4 view,
        vec3 camera_position
    ) -> void;

    [[nodiscard]] auto framebuffer() const -> Framebuffer*;
    [[nodiscard]] auto name() -> std::string&;
    [[nodiscard]] auto albedo_texture() const -> Texture*;
    [[nodiscard]] auto normal_texture() const -> Texture*;
    [[nodiscard]] auto specular_texture() const -> Texture*;
    [[nodiscard]] auto depth_texture() const -> Texture*;

  private:
    auto pass_geometry(entt::registry const& registry, DrawDestination destination, mat4 projection, mat4 view) -> void;
    auto pass_lighting(
        DrawDestination draw_destination,
        entt::registry const& registry,
        mat4 projection,
        mat4 view,
        vec3 camera_position
    ) -> void;

  private:
    std::unique_ptr<Framebuffer> _gbuffers;
    std::string _name;
};

}  // namespace engine