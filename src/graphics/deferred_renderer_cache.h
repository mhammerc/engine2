#pragma once

#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "deferred_renderer.h"

namespace engine {

/**
 * See TextureLoader for documentation on the loading paradigm.
 */
struct DeferredRendererLoader {
    using result_type = std::shared_ptr<DeferredRenderer>;

    auto operator()(std::unique_ptr<DeferredRenderer> deferred_renderer) const -> result_type;

    auto operator()(std::function<std::unique_ptr<DeferredRenderer>(void)> const& deferred_loading) const
        -> result_type;
};

using DeferredRendererCache = entt::resource_cache<DeferredRenderer, DeferredRendererLoader>;

}  // namespace engine