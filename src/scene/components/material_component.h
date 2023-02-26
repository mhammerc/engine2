#pragma once

#include <map>
#include <memory>

#include "../../common.h"
#include "../../graphics/mesh.h"
#include "../../graphics/shader_program.h"
#include "../../graphics/texture.h"
#include "../component.h"

namespace engine {

class MaterialComponent: public Component {
  public:
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texCoords;
    };

    auto shininess() -> float&;
    auto reflection() -> float&;
    auto refraction() -> float&;

    auto shader_update_uniforms() -> void;
    auto bind() -> void;
    auto unbind() -> void;

    auto textures() -> std::map<i32, std::shared_ptr<Texture>>&;
    auto shader() -> std::shared_ptr<ShaderProgram>&;
    auto mesh() -> std::shared_ptr<Mesh>&;

  private:
    float _shininess = 256.F;
    float _reflection = 0.F;
    float _refraction = 0.F;

    std::map<i32, std::shared_ptr<Texture>> _textures;
    std::shared_ptr<ShaderProgram> _shader;
    std::shared_ptr<Mesh> _mesh;
};

}  // namespace engine