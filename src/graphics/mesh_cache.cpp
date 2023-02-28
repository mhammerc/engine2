#include "mesh_cache.h"

using namespace engine;

auto MeshLoader::operator()(std::vector<Mesh::Vertex>&& vertices, std::vector<u32>&& indices) const -> result_type {
    return std::make_shared<Mesh>(std::move(vertices), std::move(indices));
}

auto MeshLoader::operator()(std::shared_ptr<Mesh> mesh) const -> result_type {
    return mesh;
}