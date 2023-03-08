#pragma once

#include "../../common.h"

namespace engine {
struct LightComponent {
    enum Type {
        Unset = 0x0000,
        Directional = 0x0001,
        Point = 0x0002,
        Spot = 0x0003,
    };

    Type type = Type::Point;

    // Color
    vec3 ambient = vec3(0.35F);
    vec3 diffuse = vec3(0.8F);
    vec3 specular = vec3(1.F);

    // Attenuation
    float constant = 1.F;  // spot, point
    float linear = 0.09F;  // spot, point
    float quadratic = 0.032F;  // spot, point

    // Spotlight
    float inner_cut_off = 0.F;  // spot
    float outer_cut_off = 0.F;  // spot

    bool draw_gizmo = true;
};

namespace reflection {
    auto register_light_component() -> void;
}
}  // namespace engine