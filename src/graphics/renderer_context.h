#pragma once

#include <array>

#include "../common.h"

namespace engine {
/**
 * The `RendererContext` store all the needed data for each renderer to complete its job.
 */
struct RendererContext {
    mat4 projection;
};
}  // namespace engine