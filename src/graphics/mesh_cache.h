#pragma once

#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "../core/noncopyable.h"
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

class MeshCache: public entt::resource_cache<Mesh, MeshLoader>, public noncopyable {};

}  // namespace engine