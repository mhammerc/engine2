#pragma once

#include "texture.h"

namespace engine {

struct texture_loader {
    using result_type = std::shared_ptr<Texture>;

    auto
    operator()(std::string const& name, const std::filesystem::path& path, Texture::Type type, bool flip = true) const
        -> result_type;
    auto operator()(std::string const& name, Texture::Type type, vec2i size, int multisample) const -> result_type;
};

}  // namespace engine