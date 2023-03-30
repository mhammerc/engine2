#pragma once

#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "../core/noncopyable.h"
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

class FramebufferCache: public entt::resource_cache<Framebuffer, FramebufferLoader>, public noncopyable {};

}  // namespace engine