#pragma once

#include <map>

#include "../../common.h"
#include "../../graphics/opengl/texture.h"
#include "../component.h"

class MaterialComponent: public Component {
  public:
    auto shininess() -> float&;
    auto reflection() -> float&;
    auto refraction() -> float&;

    auto textures() -> std::map<i32, Texture*>&;

  private:
    float _shininess = 256.F;
    float _reflection = 0.F;
    float _refraction = 0.F;

    std::map<i32, Texture*> _textures;
};