#pragma once

#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "framebuffer.h"

namespace engine {

/**
 * See TextureLoader for documentation on the loading paradigm.
 */
struct FramebufferLoader {
    using result_type = std::shared_ptr<Framebuffer>;

    auto operator()(std::unique_ptr<Framebuffer> framebuffer) const -> result_type;

    auto operator()(std::function<std::unique_ptr<Framebuffer>(void)> const& deferred_loading) const -> result_type;
};

using FramebufferCache = entt::resource_cache<Framebuffer, FramebufferLoader>;

}  // namespace engine