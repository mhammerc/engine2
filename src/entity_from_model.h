#pragma once

#include <entt/entt.hpp>
#include <filesystem>
#include <memory>
#include <vector>

namespace engine {

/**
 * This function will load a 3D model using Assimp.
 *
 * From the assimp scene, a complete hierarchy of GameObject(s) will be
 * created alongside their materials and textures.
 */
auto entity_from_model(const std::filesystem::path& path, entt::registry& registry) -> std::vector<entt::entity>;

}  // namespace engine