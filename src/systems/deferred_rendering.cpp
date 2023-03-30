#include "../components/base_component.h"
#include "../components/camera_component.h"
#include "../components/deferred_renderer_component.h"
#include "../graphics/deferred_renderer/deferred_renderer_cache.h"
#include "systems.h"

using namespace engine;

/**
 * Delete DeferredRenderers that don't have components existing anymore.
 */
static auto delete_orphans_renderers(entt::registry& registry) -> void {
    auto& deferred_renderer_cache = entt::locator<DeferredRendererCache>::value();

    for (auto [entity_id, deferred_renderer] : deferred_renderer_cache) {
        auto entity = static_cast<entt::entity>(entity_id);

        if (!registry.valid(entity)) {
            // The deferred renderer is orphan: no component make use of it anymore.
            // TODO: TO BE TESTED
            deferred_renderer_cache.erase(entity_id);
        }
    }
}

static auto perform_deferred_rendering(
    entt::registry& registry,
    entt::entity entity,
    BaseComponent const& base,
    DeferredRendererComponent const& renderer_component
) -> void {
    auto& deferred_renderer_cache = entt::locator<DeferredRendererCache>::value();

    auto renderer_id = static_cast<entt::id_type>(entity);
    auto [renderer_resource, _0] =
        deferred_renderer_cache.load(renderer_id, [&]() { return DeferredRenderer::create(base.name); });
    auto renderer = renderer_resource->second;

    if (renderer->name() != base.name) {
        // Update the name of the renderer to the name of the entity
        renderer->name() = base.name;
    }

    auto [camera_base, camera] = registry.get<BaseComponent, CameraComponent>(renderer_component.camera);
    auto camera_transform = camera_base.world_transform(registry);
    auto view = camera.view_matrix(camera_base);

    // The size and position of the renderer component are in range [0;1]. Scale these ranges to pixels.
    vec2i size = renderer_component.size * static_cast<vec2>(renderer_component.destination->size());
    vec2i position = renderer_component.position * static_cast<vec2>(renderer_component.destination->size());

    auto projection = glm::perspective(
        glm::radians(camera.fov),
        static_cast<float>(size.x) / static_cast<float>(size.y),
        camera.zNear,
        camera.zFar
    );

    auto draw_destination = DeferredRenderer::DrawDestination {
        .framebuffer = renderer_component.destination,
        .position = position,
        .size = size};

    auto renderer_context = RendererContext {
        .projection = projection,
        .view = view,
        .camera = renderer_component.camera,
        .camera_transform = camera_transform,
        .wireframe = renderer_component.wireframe};

    renderer->draw(registry, draw_destination, renderer_context);
}

/**
 * This system perform many tasks.
 *
 * DeferredRendererComponent is purely the data of the renderer, encapsulated into the ECS paradigm.
 *
 * To perform deferred rendering through GPU, we maintain the "GPU" renderer from the DeferredRendererCache. We must
 * keep in sync the component and the renderer.
 *
 * 1) Update the DeferredRenderers from the DeferredRendererComponents:
 *   a) Clean the DeferredRenderers which component have been deleted
 *   b) Create the DeferredRenderers for new components
 *   c) Update the DeferredRenderers name to its component new name, if changed.
 * 2) Render
 */
auto systems::draw_render_deferred(entt::registry& registry) -> void {
    delete_orphans_renderers(registry);

    auto view = registry.view<BaseComponent, DeferredRendererComponent>().each();
    for (auto const& [entity, base, renderer_component] : view) {
        if (!base.enabled) {
            continue;
        }

        perform_deferred_rendering(registry, entity, base, renderer_component);
    }
}