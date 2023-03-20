#include "framebuffer_cache.h"

using namespace engine;

auto FramebufferLoader::operator()(std::unique_ptr<Framebuffer> framebuffer) const -> result_type {
    return std::move(framebuffer);
}

auto FramebufferLoader::operator()(std::function<std::unique_ptr<Framebuffer>(void)> const& deferred_loading) const
    -> result_type {
    auto resource = deferred_loading();

    return std::move(resource);
}