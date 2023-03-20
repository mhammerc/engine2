#pragma once

#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "mesh.h"

namespace engine {

/**
 * See TextureLoader for documentation on the loading paradigm.
 */
struct MeshLoader {
    using result_type = std::shared_ptr<Mesh>;

    auto operator()(std::unique_ptr<Mesh> mesh) const -> result_type;

    auto operator()(std::function<std::unique_ptr<Mesh>(void)> const& deferred_loading) const -> result_type;
};

using MeshCache = entt::resource_cache<Mesh, MeshLoader>;

}  // namespace engine