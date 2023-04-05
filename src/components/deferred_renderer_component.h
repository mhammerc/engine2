#pragma once

#include <entt/entt.hpp>

#include "../common.h"
#include "../graphics/framebuffer.h"
#include "../graphics/renderer_context.h"

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

    /**
     * Exposure of the camera. Higher value will show more details in dark areas.
     */
    float exposure = 1.F;

    /**
     * What tone mapping to use?
     */
    RendererContext::ToneMapping tone_mapping = RendererContext::ToneMapping::AcesApprox;

    /**
     * Enable bloom?
     */
    bool bloom_enabled = true;

    /**
     * How much the bloom is wide?
     */
    float bloom_radius = 0.2F;

    /**
     * Bloom intensity
     */
    float bloom_intensity = 0.1F;
};

namespace reflection {
    auto register_deferred_renderer_component() -> void;
}

}  // namespace engine