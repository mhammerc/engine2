#pragma once

#include <entt/entt.hpp>

#include "../common.h"
#include "../graphics/framebuffer.h"

namespace engine {

struct DeferredRendererComponent {
    /**
     * Where to render?
     */
    Framebuffer* destination;

    /**
     * Where to render on the destination framebuffer?
     *
     * In range [0;1]
     */
    vec2 position = {0.F, 0.F};

    /**
     * What size to render on the destination framebuffer?
     *
     * In range [0;1]
     */
    vec2 size = {1.F, 1.F};

    /**
     * What is the camera to render from?
     */
    entt::entity camera;

    /**
     * Should render in wireframe?
     */
    bool wireframe = false;
};

namespace reflection {
    auto register_deferred_renderer_component() -> void;
}

}  // namespace engine