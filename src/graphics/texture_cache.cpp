#include "texture_cache.h"

using namespace engine;

auto TextureLoader::operator()(std::unique_ptr<Texture> from) const -> TextureLoader::result_type {
    return std::move(from);
}
