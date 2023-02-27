#include "scene.h"

#include <spdlog/fmt/fmt.h>

#include "../core/locator.h"
#include "../graphics/renderer_context.h"
#include "../ui/gui.h"
#include "components/renderer_component.h"
#include "components/transform_component.h"
#include "game_object_from_model.h"

using namespace engine;

Scene::Scene() {
    world.name() = "World";

    auto backpack = game_object_from_model("../assets/backpack/backpack.obj");
    backpack->name() = "backpack";
    backpack->component<TransformComponent>()->position() = vec3(0.F, 0.F, -5.F);

    auto floor = game_object_from_model("../assets/plane/plane.obj");
    floor->name() = "floor";
    floor->component<TransformComponent>()->position() = vec3(0.F, -5.F, 0.F);

    world.childrens().push_back(std::move(backpack));
    world.childrens().push_back(std::move(floor));
}

auto Scene::draw(GLFWwindow* window, float delta_time, Skybox* /*skybox*/) -> void {
    gui_show_system_window(this, delta_time, window);

    camera->computeFront();

    lights.at(9) = Light {
        .type = flashlight ? Light::Spot : Light::Unset,
        .position = camera->pos,
        .direction = camera->front,
        .innerCutOff = glm::cos(glm::radians(12.5F)),
        .outerCutOff = glm::cos(glm::radians(17.5F)),
        .linear = 0.09F,
        .quadratic = 0.032F,
        .ambient = glm::vec3(0.2F, 0.2F, 0.2F),
        .diffuse = glm::vec3(0.5F, 0.5F, 0.5F),
        .specular = glm::vec3(0.0F, 0.0F, 0.0F)};

    if (outline) {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto* renderer_context = locator<RendererContext>::value();
    renderer_context->lights = lights;
    draw_nodes(&world);

    // if (show_normals) {
    //     draw_nodes_normals(projection);
    // }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_STENCIL_TEST);
    // skybox->draw(projection, camera->getMatrix());

    // if (outline) {
    //     glEnable(GL_STENCIL_TEST);
    //     glStencilMask(0x00);
    //     glDisable(GL_DEPTH_TEST);
    //     glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    //     draw_nodes_outline(projection);

    //     glEnable(GL_DEPTH_TEST);
    //     glDisable(GL_STENCIL_TEST);
    //     glStencilMask(0xFF);
    // }
}

auto Scene::draw_nodes(GameObject* game_object) -> void {
    auto* renderer = game_object->component<RendererComponent>();

    if (renderer) {
        renderer->on_draw();
    }

    for (auto& child : game_object->childrens()) {
        draw_nodes(child.get());
    }
}

auto Scene::draw_nodes_outline() -> void {
    // for (auto& node : nodes) {
    //     auto previousShader = node.shader();
    //     node.shader() = outline_shader;

    //     node.shader()->set_uniform("view", camera->getMatrix());
    //     node.shader()->set_uniform("projection", projection);

    //     node.draw();

    //     node.shader() = previousShader;
    // }
}

auto Scene::draw_nodes_normals() -> void {
    // for (auto& node : nodes) {
    //     auto previousShader = node.shader();
    //     node.shader() = normal_shader;

    //     node.shader()->set_uniform("view", camera->getMatrix());
    //     node.shader()->set_uniform("projection", projection);

    //     node.draw();

    //     node.shader() = previousShader;
    // }
}
