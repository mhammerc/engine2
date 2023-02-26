#pragma once

#include <filesystem>
#include <memory>

#include "game_object.h"

namespace engine {

/**
 * This function will load a 3D model using Assimp.
 *
 * From the assimp scene, a complete hierarchy of GameObject(s) will be
 * created alongside their materials and textures.
 */
auto game_object_from_model(const std::filesystem::path& path) -> std::unique_ptr<GameObject>;

}  // namespace engine