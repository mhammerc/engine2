#pragma once

#include <functional>
#include <memory>

#include "entt/entt.hpp"
#include "texture.h"

namespace engine {

struct TextureLoader {
    using result_type = std::shared_ptr<Texture>;

    /**
     * Use as follow:
     *
     * ```cpp
     * cache.load(id, Texture::from_XXX());
     * ```
     *
     * Use this overload only if you are sure the resource is going to be loaded.
     * If you "get or load if not found", use the other overload.
     */
    auto operator()(std::unique_ptr<Texture> from) const -> result_type;

    /**
     * Use as follow:
     *
     * ```cpp
     * cache.load(id, []() { return Texture::from_XXX(); });
     * ```
     *
     * The deferred function is called only if the resource is not found.
     *
     * This overload allow for a deferred loading of the resource. If you are "get the resource or load it if not found",
     * you must use this overload to avoid unwanted construction then destruction of resources.
     */
    auto operator()(std::function<std::unique_ptr<Texture>(void)> const& deferred_loading) const -> result_type;
};

using TextureCache = entt::resource_cache<Texture, TextureLoader>;

}  // namespace engine