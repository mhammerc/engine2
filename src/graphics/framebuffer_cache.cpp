#include "framebuffer_cache.h"

#include <utility>

#include "framebuffer.h"

using namespace engine;

auto FrameBufferLoader::operator()(std::string name, vec2i size, FrameBuffer::Type type) const -> result_type {
    auto a = FrameBuffer::create(size, type);

    return std::make_shared<FrameBufferCached>(std::make_pair(std::move(name), std::move(*a)));
}