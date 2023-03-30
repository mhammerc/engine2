#pragma once

#include <functional>
#include <memory>

#include "../../core/noncopyable.h"
#include "deferred_renderer.h"
#include "entt/entt.hpp"

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

class DeferredRendererCache:
    public entt::resource_cache<DeferredRenderer, DeferredRendererLoader>,
    public noncopyable {};

}  // namespace engine