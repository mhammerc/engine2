#include "deferred_renderer_cache.h"

using namespace engine;

auto DeferredRendererLoader::operator()(std::unique_ptr<DeferredRenderer> deferred_renderer) const -> result_type {
    return std::move(deferred_renderer);
}

auto DeferredRendererLoader::operator()(std::function<std::unique_ptr<DeferredRenderer>(void)> const& deferred_loading
) const -> result_type {
    auto resource = deferred_loading();

    return std::move(resource);
}