#include "cube_map_cache.h"

using namespace engine;

auto CubeMapLoader::operator()(const std::array<std::filesystem::path, 6>& files) const -> result_type {
    return CubeMap::from_files(files);
}