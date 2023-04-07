#include "texture_cache.h"

using namespace engine;

auto TextureLoader::operator()(std::unique_ptr<Texture> from) const -> result_type {
    return from;
}

auto TextureLoader::operator()(std::function<std::unique_ptr<Texture>(void)> const& deferred_loading) const
    -> result_type {
    auto resource = deferred_loading();

    return resource;
}