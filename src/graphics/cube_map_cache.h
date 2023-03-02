#pragma once

#include <entt/entt.hpp>

#include "cube_map.h"

namespace engine {

struct CubeMapLoader {
    using result_type = std::shared_ptr<CubeMap>;

    auto operator()(std::array<std::filesystem::path, 6> const& files) const -> result_type;
};

using CubeMapCache = entt::resource_cache<CubeMap, CubeMapLoader>;

}  // namespace engine