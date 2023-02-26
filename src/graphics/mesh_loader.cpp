#include "mesh_loader.h"

using namespace engine;

auto MeshLoader::operator()(
    const std::string& /*name*/,
    std::vector<Mesh::Vertex>&& vertices,
    std::vector<u32>&& indices
) const -> result_type {
    return std::make_shared<Mesh>(std::move(vertices), std::move(indices));
}