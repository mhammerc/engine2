#pragma once

#include "../common.h"

namespace engine {

/**
 * Sometime we need an OpenGL object handle as a void* pointer (ex: ImGui).
 *
 * Use this function only for these limited cases.
 */
auto opengl_handle_to_pointer(u32 handle) -> void*;

}  // namespace engine
