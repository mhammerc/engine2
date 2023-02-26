#pragma once

#include <array>

#include "../common.h"
#include "../scene/camera.h"
#include "../scene/light.h"

namespace engine {
/**
 * The `RendererContext` store all the needed data for each renderer to complete its job.
 */
struct RendererContext {
    mat4 projection;
    std::array<Light, 10> lights;
    Camera* camera;
};
}  // namespace engine