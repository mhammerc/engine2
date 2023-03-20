#pragma once

#include <map>
#include <memory>

#include "../../common.h"
#include "../../graphics/mesh.h"
#include "../../graphics/shader_program.h"
#include "../../graphics/texture/texture.h"

namespace engine {

struct MaterialComponent {
    float shininess = 256.F;
    float reflection = 0.F;
    float refraction = 0.F;

    std::map<i32, std::shared_ptr<Texture>> textures;
    std::shared_ptr<ShaderProgram> shader;
    std::shared_ptr<Mesh> mesh;
};

namespace reflection {
    auto register_material_component() -> void;
}

}  // namespace engine