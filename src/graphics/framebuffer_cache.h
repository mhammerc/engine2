#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <string>
#include <tuple>

#include "framebuffer.h"

namespace engine {
// Add a name to framebuffers
using FrameBufferCached = std::pair<std::string, FrameBuffer>;

struct FrameBufferLoader {
    using result_type = std::shared_ptr<FrameBufferCached>;

    auto operator()(std::string name, vec2i size, FrameBuffer::Type type) const -> result_type;
};

using FrameBufferCache = entt::resource_cache<FrameBufferCached, FrameBufferLoader>;
}  // namespace engine