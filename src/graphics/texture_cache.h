#pragma once

#include <entt/entt.hpp>
#include <memory>

#include "texture.h"

namespace engine {

struct TextureLoader {
    using result_type = std::shared_ptr<Texture>;

    /**
     * Use as follow:
     *
     * ```cpp
     * cache.load(Texture::from_XXX());
     * ```
     */
    auto operator()(std::unique_ptr<Texture> from) const -> result_type;
};

using TextureCache = entt::resource_cache<Texture, TextureLoader>;

}  // namespace engine