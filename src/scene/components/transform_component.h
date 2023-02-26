#pragma once

#include "../../common.h"
#include "../component.h"

class TransformComponent: public Component {
  public:
    auto position() -> vec3&;
    auto rotation() -> vec3&;
    auto scale() -> vec3&;

  private:
    vec3 _position = vec3(0.F);
    vec3 _rotation = vec3(0.F);
    vec3 _scale = vec3(1.F);
};