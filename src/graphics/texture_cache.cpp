#include "texture_cache.h"

using namespace engine;

auto TextureLoader::operator()(const std::filesystem::path& path, Texture::Type type, bool flip) const
    -> TextureLoader::result_type {
    auto texture = Texture::from_file(path, type, flip);

    return std::move(texture);
}

auto TextureLoader::operator()(std::string const& name, Texture::Type type, vec2i size, int multisample) const
    -> TextureLoader::result_type {
    auto texture = Texture::from_empty(name, type, size, multisample);

    return std::move(texture);
}