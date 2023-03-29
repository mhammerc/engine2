#pragma once

#include <array>
#include <entt/entt.hpp>

#include "../common.h"
#include "../core/transform.h"

namespace engine {

/**
 * The `RendererContext` store all the needed data for each renderer to complete.
 */
struct RendererContext {
    struct PostProcessing {
        static u32 constexpr POST_PROCESS_INVERSE = (1 << 0);
        static u32 constexpr POST_PROCESS_GRAYSCALE = (1 << 1);
        static u32 constexpr POST_PROCESS_SEPIA = (1 << 2);
        static u32 constexpr POST_PROCESS_BLUR = (1 << 3);
        static u32 constexpr POST_PROCESS_SHARPEN = (1 << 4);
        static u32 constexpr POST_PROCESS_EDGE_DETECTION = (1 << 5);

        bool inverse = false;
        bool black_and_white = false;
        bool sepia = false;
        bool blur = false;
        bool sharpen = false;
        bool edge_dectection = false;
    };

    mat4 projection;
    mat4 view;

    entt::entity camera;
    Transform camera_transform;

    bool wireframe = false;
    PostProcessing post_process;
};

}  // namespace engine