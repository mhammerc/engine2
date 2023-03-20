#include "mesh_cache.h"

using namespace engine;

auto MeshLoader::operator()(std::unique_ptr<Mesh> mesh) const -> result_type {
    return std::move(mesh);
}

auto MeshLoader::operator()(std::function<std::unique_ptr<Mesh>(void)> const& deferred_loading) const -> result_type {
    auto resource = deferred_loading();

    return std::move(resource);
}