#pragma once

#include "../common.h"

namespace engine {

struct Light {
    enum Type {
        Unset = 0x0000,
        Directional = 0x0001,
        Point = 0x0002,
        Spot = 0x0003,
    };

    Type type = Type::Unset;

    vec3 position;  // spot, point
    vec3 direction;  // spot, directional

    float innerCutOff;  // spot
    float outerCutOff;  // spot

    float constant = 1.F;  // spot, point
    float linear;  // spot, point
    float quadratic;  // spot, point

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

}  // namespace engine