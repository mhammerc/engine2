#pragma once

#include "../../graphics/framebuffer.h"
#include "../../graphics/texture/texture.h"

namespace engine::ui::internal {

auto draw_framebuffer_viewer() -> void;
auto draw_texture_viewer() -> void;
auto draw_cubemap_viewer() -> void;

enum UnfoldedOrEquirectangular { Unfolded, Equirectangular };
auto draw_cubemap_to_framebuffer(Texture& cubemap, UnfoldedOrEquirectangular mode) -> Framebuffer*;

}  // namespace engine::ui::internal