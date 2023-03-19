#pragma once

#include <entt/entt.hpp>
#include <memory>

#include "framebuffer.h"

namespace engine {
struct FramebufferLoader {
    using result_type = std::shared_ptr<Framebuffer>;

    auto operator()(std::unique_ptr<Framebuffer> framebuffer) const -> result_type;
};

using FramebufferCache = entt::resource_cache<Framebuffer, FramebufferLoader>;
}  // namespace engine