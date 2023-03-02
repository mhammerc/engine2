#pragma once

#include <entt/entt.hpp>
#include <string>

#include "mesh.h"

namespace engine {

struct MeshLoader {
    using result_type = std::shared_ptr<Mesh>;

    auto operator()(std::string const& name, std::vector<Mesh::Vertex>&& vertices, std::vector<u32>&& indices) const
        -> result_type;
    auto operator()(std::shared_ptr<Mesh> mesh) const -> result_type;
};

using MeshCache = entt::resource_cache<Mesh, MeshLoader>;

}  // namespace engine