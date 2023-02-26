#include "scene.h"

#include <spdlog/fmt/fmt.h>

#include "../ui/gui.h"

Scene::Scene() {
    world.name() = "World";

    auto bag = std::make_unique<GameObject>("bag");
    auto player = std::make_unique<GameObject>("player");
    player->childrens().emplace_back(std::make_unique<GameObject>("camera"));
    auto skybox = std::make_unique<GameObject>("skybox");
    auto floor = std::make_unique<GameObject>("floor");

    world.childrens().push_back(std::move(bag));
    world.childrens().push_back(std::move(player));
    world.childrens().push_back(std::move(skybox));
    world.childrens().push_back(std::move(floor));
}

auto Scene::draw(GLFWwindow* window, float delta_time, glm::mat4 projection, Skybox* skybox) -> void {
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

    draw_nodes(projection);

    if (show_normals) {
        draw_nodes_normals(projection);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_STENCIL_TEST);
    skybox->draw(projection, camera->getMatrix());

    if (outline) {
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

        draw_nodes_outline(projection);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
    }
}

auto Scene::draw_nodes(glm::mat4 projection) -> void {
    for (auto& node : nodes) {
        node.shader()->set_uniform("view", camera->getMatrix());
        node.shader()->set_uniform("projection", projection);
        node.shader()->set_uniform("cameraPosition", camera->pos);
        node.shader()->set_uniform("material.shininess", 256.0F);
        node.shader()->set_uniform("material.texture_environment", 10);
        node.shader()->set_uniform("material.reflect", mirror ? 1.F : 0.F);
        node.shader()->set_uniform("material.refract", glass ? 1.F : 0.F);
        node.shader()->set_uniform("explosion", explosion);

        for (size_t i = 0; i < 10; ++i) {
            Light const& light = lights.at(i);

            node.shader()->set_uniform(fmt::format("lights[{}].type", i), static_cast<int>(light.type));
            node.shader()->set_uniform(fmt::format("lights[{}].position", i), light.position);
            node.shader()->set_uniform(fmt::format("lights[{}].direction", i), glm::normalize(light.direction));
            node.shader()->set_uniform(fmt::format("lights[{}].innerCutOff", i), light.innerCutOff);
            node.shader()->set_uniform(fmt::format("lights[{}].outerCutOff", i), light.outerCutOff);
            node.shader()->set_uniform(fmt::format("lights[{}].constant", i), light.constant);
            node.shader()->set_uniform(fmt::format("lights[{}].linear", i), light.linear);
            node.shader()->set_uniform(fmt::format("lights[{}].quadratic", i), light.quadratic);
            node.shader()->set_uniform(fmt::format("lights[{}].ambient", i), light.ambient);
            node.shader()->set_uniform(fmt::format("lights[{}].diffuse", i), light.diffuse);
            node.shader()->set_uniform(fmt::format("lights[{}].specular", i), light.specular);
        }

        node.draw();
    }
}

auto Scene::draw_nodes_outline(glm::mat4 projection) -> void {
    for (auto& node : nodes) {
        auto previousShader = node.shader();
        node.shader() = outline_shader;

        node.shader()->set_uniform("view", camera->getMatrix());
        node.shader()->set_uniform("projection", projection);

        node.draw();

        node.shader() = previousShader;
    }
}

auto Scene::draw_nodes_normals(glm::mat4 projection) -> void {
    for (auto& node : nodes) {
        auto previousShader = node.shader();
        node.shader() = normal_shader;

        node.shader()->set_uniform("view", camera->getMatrix());
        node.shader()->set_uniform("projection", projection);

        node.draw();

        node.shader() = previousShader;
    }
}
