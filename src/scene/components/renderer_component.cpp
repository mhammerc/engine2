#include "renderer_component.h"

#include <entt/entt.hpp>

#include "../../graphics/renderer_context.h"
#include "../game_object.h"
#include "material_component.h"
#include "spdlog/spdlog.h"
#include "transform_component.h"

namespace engine {

auto RendererComponent::on_draw() -> void {
    auto* transform = game_object()->component<TransformComponent>();
    auto* material = game_object()->component<MaterialComponent>();
    auto& renderer_context = entt::locator<RendererContext>::value();

    if (!transform || !material) {
        spdlog::error(
            "[GameObject {}] RendererComponent::on_draw: GameObject must have Transform and Material components."
        );
    }

    const auto model = transform->matrix();
    const auto modelNormal = mat3(glm::transpose(glm::inverse(model)));

    // MVP
    material->shader()->set_uniform("model", model);
    material->shader()->set_uniform("modelNormal", modelNormal);
    material->shader()->set_uniform("view", renderer_context.camera->getMatrix());
    material->shader()->set_uniform("projection", renderer_context.projection);

    material->shader()->set_uniform("cameraPosition", renderer_context.camera->pos);

    for (size_t i = 0; i < 10; ++i) {
        Light const& light = renderer_context.lights.at(i);

        material->shader()->set_uniform(fmt::format("lights[{}].type", i), static_cast<int>(light.type));
        material->shader()->set_uniform(fmt::format("lights[{}].position", i), light.position);
        material->shader()->set_uniform(fmt::format("lights[{}].direction", i), glm::normalize(light.direction));
        material->shader()->set_uniform(fmt::format("lights[{}].innerCutOff", i), light.innerCutOff);
        material->shader()->set_uniform(fmt::format("lights[{}].outerCutOff", i), light.outerCutOff);
        material->shader()->set_uniform(fmt::format("lights[{}].constant", i), light.constant);
        material->shader()->set_uniform(fmt::format("lights[{}].linear", i), light.linear);
        material->shader()->set_uniform(fmt::format("lights[{}].quadratic", i), light.quadratic);
        material->shader()->set_uniform(fmt::format("lights[{}].ambient", i), light.ambient);
        material->shader()->set_uniform(fmt::format("lights[{}].diffuse", i), light.diffuse);
        material->shader()->set_uniform(fmt::format("lights[{}].specular", i), light.specular);
    }

    material->shader_update_uniforms();

    material->bind();
    material->mesh()->draw();
    material->unbind();
}

}  // namespace engine