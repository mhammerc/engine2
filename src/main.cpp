#include <entt/entt.hpp>

#include "core/game_loop.h"
#include "entt/entity/fwd.hpp"
#include "graphics/framebuffer.h"
#include "graphics/mesh_cache.h"
#include "graphics/renderer_context.h"
#include "graphics/shader_cache.h"
#include "graphics/shader_program.h"
#include "graphics/skybox.h"
#include "graphics/texture_cache.h"
#include "platform/glfw.h"
#include "platform/opengl.h"
#include "scene/camera.h"
#include "scene/game_object.h"
#include "scene/scene.h"
#include "spdlog/spdlog.h"
#include "stb_image/stb_image.h"
#include "ui/gui.h"

using namespace engine;

auto main() -> int {
    auto registry = entt::registry();

    auto entity = registry.create();
    (void)entity;

    auto* window = init_glfw_and_opengl();
    if (window == nullptr) {
        spdlog::critical("Could not initialize GLFW3 or OpenGL.");
        return 1;
    }

    if (!gui_init(window)) {
        spdlog::critical("Could not initialize GUI.");
        return 1;
    }

    auto& shader_cache = entt::locator<engine::ShaderCache>::emplace();
    entt::locator<engine::TextureCache>::emplace();
    auto& mesh_cache = entt::locator<engine::MeshCache>::emplace();

    auto& renderer_context = entt::locator<engine::RendererContext>::emplace();

    auto [diffuse_shader, _] = shader_cache.load("diffuse"_hs, "diffuse");
    if (diffuse_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    auto [light_source_shader, _2] = shader_cache.load("light"_hs, "light");
    if (light_source_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    auto [outline_shader, _3] = shader_cache.load("outline"_hs, "outline");
    if (outline_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    auto [postprocess_shader, _4] = shader_cache.load("postprocess"_hs, "postprocess");
    if (postprocess_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    auto [normal_shader, _5] = shader_cache.load("normal"_hs, "normal");
    if (normal_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    Scene scene;
    scene.camera = std::make_shared<Camera>(window);
    renderer_context.camera = scene.camera.get();

    {
        auto const pointLight = Light {
            .type = Light::Point,
            .position = glm::vec3(0.F, 0.F, -2.F),
            .linear = 0.09F,
            .quadratic = 0.032F,
            .ambient = glm::vec3(0.35F, 0.35F, 0.35F),
            .diffuse = glm::vec3(0.8F, 0.8F, 0.8F),
            .specular = glm::vec3(1.F, 1.F, 1.F),
        };
        scene.lights.at(0) = pointLight;
    }

    auto skybox = Skybox::from_files({
        "../assets/skybox/right.jpg",
        "../assets/skybox/left.jpg",
        "../assets/skybox/top.jpg",
        "../assets/skybox/bottom.jpg",
        "../assets/skybox/front.jpg",
        "../assets/skybox/back.jpg",
    });

    vec2i size(0);
    glfwGetFramebufferSize(window, &size.x, &size.y);
    auto frame_buffer = FrameBuffer::create(size);

    auto mesh_quad = mesh_cache.load("quad"_hs, Mesh::from_quad());

    engine::game_loop([&](float delta_time, bool& should_quit) {
        if (glfwWindowShouldClose(window) != 0) {
            should_quit = true;
        }

        gui_prepare_frame();

        process_inputs(delta_time, window, *scene.camera);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwGetFramebufferSize(window, &size.x, &size.y);
        renderer_context.projection = glm::perspective(
            glm::radians(45.0F),
            static_cast<float>(size.x) / static_cast<float>(size.y),
            0.1F,
            100.0F
        );

        // Resize the framebuffer as needed
        frame_buffer->resize(size);

        // Render scene
        {
            frame_buffer->bind();

            glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            skybox->activate_cubemap_as(10);
            scene.draw(window, delta_time, skybox.get());

            frame_buffer->unbind();
        }

        // Render postprocess quad
        {
            glDisable(GL_DEPTH_TEST);

            frame_buffer->color_texture()->activate_as(0);
            auto _postprocess_shader = shader_cache["postprocess"_hs];

            _postprocess_shader->set_uniform("screenTexture", 0);

            int post_process = 0;
            if (scene.inverse) {
                post_process |= POST_PROCESS_INVERSE;
            }
            if (scene.black_and_white) {
                post_process |= POST_PROCESS_GRAYSCALE;
            }
            if (scene.sepia) {
                post_process |= POST_PROCESS_SEPIA;
            }
            if (scene.blur) {
                post_process |= POST_PROCESS_BLUR;
            }
            if (scene.sharpen) {
                post_process |= POST_PROCESS_SHARPEN;
            }
            if (scene.edge_dectection) {
                post_process |= POST_PROCESS_EDGE_DETECTION;
            }
            _postprocess_shader->set_uniform("post_process", post_process);

            _postprocess_shader->bind();
            mesh_quad.first->second->draw();
            _postprocess_shader->unbind();

            glEnable(GL_DEPTH_TEST);
        }

        gui_end_frame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    });

    terminate_glfw(window);

    return 0;
}
