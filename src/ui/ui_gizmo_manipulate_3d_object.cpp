#include <glm/gtx/matrix_decompose.hpp>

#include "../components/base_component.h"
#include "../components/camera_component.h"
#include "../components/deferred_renderer_component.h"
#include "ui_internal.h"

using namespace engine;

static auto draw_widget(
    entt::registry& registry,
    ui::internal::GizmoManipulate config,
    BaseComponent& selected_entity,
    DeferredRendererComponent renderer
) -> void {
    auto [camera_base, camera] = registry.get<BaseComponent, CameraComponent>(renderer.camera);
    auto view = camera.view_matrix(camera_base);

    auto projection = glm::perspective(
        glm::radians(camera.fov),
        (config.size.x * renderer.size.x) / (config.size.y * renderer.size.y),
        camera.zNear,
        camera.zFar
    );

    auto model = selected_entity.world_transform(registry).matrix();

    ImGuizmo::SetRect(
        config.position.x + (config.size.x * renderer.position.x),
        config.position.y + (config.size.y * renderer.position.y * -1.F) + (1.F - renderer.size.y) * config.size.y,
        config.size.x * renderer.size.x,
        config.size.y * renderer.size.y
    );
    ImGuizmo::SetDrawlist();

    mat4 delta_matrix(1.F);
    ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(projection),
        config.operation,
        config.mode,
        glm::value_ptr(model),
        glm::value_ptr(delta_matrix)
    );

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 _skew;
    glm::vec4 _perspective;
    glm::decompose(delta_matrix, scale, rotation, translation, _skew, _perspective);

    // protect against inf and nan
    if (config.operation == ImGuizmo::OPERATION::TRANSLATE && !glm::any(glm::isnan(translation))
        && !glm::any(glm::isinf(translation))) {
        selected_entity.transform.position += translation;
    }
    if (config.operation == ImGuizmo::OPERATION::ROTATE && !glm::any(glm::isnan(rotation))
        && !glm::any(glm::isinf(rotation))) {
        selected_entity.transform.rotation = glm::normalize(selected_entity.transform.rotation * rotation);
    }
    if (config.operation == ImGuizmo::OPERATION::SCALE && !glm::any(glm::isnan(scale))
        && !glm::any(glm::isinf(scale))) {
        selected_entity.transform.scale *= scale;
    }
}

auto ui::internal::ui_gizmo_manipulate_3d_object(
    entt::registry& registry,
    GizmoManipulate config,
    entt::entity selected_entity
) -> void {
    if (selected_entity == entt::null) {
        return;
    }

    auto* selected_entity_base = registry.try_get<BaseComponent>(selected_entity);

    if (!selected_entity_base) {
        return;
    }

    for (auto [_entity, renderer_base, renderer] : registry.view<BaseComponent, DeferredRendererComponent>().each()) {
        if (!renderer_base.enabled) {
            continue;
        }

        // Draw the widget for each rendered image of the scene.
        draw_widget(registry, config, *selected_entity_base, renderer);
    }
}