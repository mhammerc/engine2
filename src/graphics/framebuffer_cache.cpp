#include "framebuffer_cache.h"

using namespace engine;

auto FramebufferLoader::operator()(std::unique_ptr<Framebuffer> framebuffer) const -> result_type {
    return std::move(framebuffer);
}