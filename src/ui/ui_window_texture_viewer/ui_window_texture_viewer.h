#pragma once

#include <span>

#include "../../graphics/framebuffer.h"
#include "../../graphics/texture/texture.h"

namespace engine::ui::internal {

/**
 * Draw a texture. It can be 2D or cubemap.
 */
auto draw_texture(Texture* texture) -> void;

/**
 * Draw many textures.
 */
auto draw_textures(std::span<Texture*> const& textures, u32& current_index) -> void;

/**
 * Draw a framebuffer and its textures
 */
auto draw_framebuffer(Framebuffer* framebuffer, u32& texture_index) -> void;

auto draw_deferred_renderer_viewer() -> void;
auto draw_framebuffer_viewer() -> void;
auto draw_texture_viewer() -> void;

}  // namespace engine::ui::internal