#pragma once

#include "../core/resource_cache.h"
#include "texture.h"

namespace engine {

struct TextureLoader {
    using result_type = std::shared_ptr<Texture>;

    auto
    operator()(std::string const& name, const std::filesystem::path& path, Texture::Type type, bool flip = true) const
        -> result_type;
    auto operator()(std::string const& name, Texture::Type type, vec2i size, int multisample) const -> result_type;
};

using TextureCache = engine::resource_cache<Texture, TextureLoader>;

}  // namespace engine