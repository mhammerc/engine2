#include <entt/entt.hpp>

#include "core/game_loop.h"
#include "core/input.hpp"
#include "core/reflection.h"
#include "entt/entity/fwd.hpp"
#include "graphics/cube_map_cache.h"
#include "graphics/framebuffer.h"
#include "graphics/framebuffer_cache.h"
#include "graphics/mesh_cache.h"
#include "graphics/renderer_context.h"
#include "graphics/shader_cache.h"
#include "graphics/shader_program.h"
#include "graphics/texture_cache.h"
#include "platform/glfw.h"
#include "scene/components/base_component.h"
#include "scene/components/light_component.h"
#include "scene/components/outline_component.h"
#include "scene/scene.h"
#include "spdlog/spdlog.h"
#include "stb_image/stb_image.h"
#include "systems/systems.h"
#include "ui/ui.h"

using namespace engine;

auto main() -> int {
    reflection::register_all();

    auto registry = entt::registry();

    auto* window = init_glfw_and_opengl();
    if (window == nullptr) {
        spdlog::critical("Could not initialize GLFW3 or OpenGL.");
        return 1;
    }
    entt::locator<GLFWwindow*>::emplace(window);

    if (!ui_init(window)) {
        spdlog::critical("Could not initialize GUI.");
        return 1;
    }

    entt::locator<engine::TextureCache>::emplace();
    auto& shader_cache = entt::locator<engine::ShaderCache>::emplace();
    auto& mesh_cache = entt::locator<engine::MeshCache>::emplace();
    auto& cubemap_cache = entt::locator<engine::CubeMapCache>::emplace();
    auto& framebuffer_cache = entt::locator<engine::FrameBufferCache>::emplace();

    auto& renderer_context = entt::locator<engine::RendererContext>::emplace();

    entt::locator<engine::Input>::emplace();

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

    auto [skybox_shader, _7] = shader_cache.load("skybox"_hs, "skybox");
    if (skybox_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    auto [unlit_single_color_shader, _8] = shader_cache.load("unlit_single_color"_hs, "unlit_single_color");
    if (unlit_single_color_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    auto [cubemap_viewer_shader, _10] = shader_cache.load("cubemap_viewer"_hs, "cubemap_viewer");
    if (cubemap_viewer_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return 1;
    }

    cubemap_cache.load(
        "skybox"_hs,
        "skybox",
        std::array<std::filesystem::path, 6>({
            "../assets/skybox/right.jpg",
            "../assets/skybox/left.jpg",
            "../assets/skybox/top.jpg",
            "../assets/skybox/bottom.jpg",
            "../assets/skybox/front.jpg",
            "../assets/skybox/back.jpg",
        })
    );

    Scene scene(registry);

    auto light1 = registry.create();
    auto& light1_base = registry.emplace<BaseComponent>(light1, "light1");
    auto& light1_light = registry.emplace<LightComponent>(light1);
    light1_base.transform.position = vec3(0.F, 0.F, -2.F);
    light1_light.type = LightComponent::Point;
    light1_light.linear = 0.09F;
    light1_light.quadratic = 0.032F;
    light1_light.ambient = glm::vec3(0.35F, 0.35F, 0.35F);
    light1_light.diffuse = glm::vec3(0.8F, 0.8F, 0.8F);
    light1_light.specular = glm::vec3(1.F, 1.F, 1.F);

    framebuffer_cache.load("color"_hs, "color", vec2i {1, 1});
    framebuffer_cache.load("outline"_hs, "outline", vec2i {1, 1});
    framebuffer_cache.load("postprocess"_hs, "postprocess", vec2i {1, 1});
    framebuffer_cache.load("identify"_hs, "identify", vec2i {1, 1});

    mesh_cache.load("quad"_hs, Mesh::from_quad());
    mesh_cache.load("cube"_hs, Mesh::from_cube());

    engine::game_loop([&](float delta_time, bool& should_quit) {
        if (glfwWindowShouldClose(window) != 0) {
            should_quit = true;
        }

        auto* frame_buffer = &framebuffer_cache["color"_hs]->second;
        auto* frame_buffer_postprocess = &framebuffer_cache["postprocess"_hs]->second;
        auto* frame_buffer_outline = &framebuffer_cache["outline"_hs]->second;
        auto* frame_buffer_identify = &framebuffer_cache["identify"_hs]->second;

        registry.clear<OutlineComponent>();

        systems::should_close_system();
        systems::camera_system(delta_time, registry);

        ui_prepare_frame();
        ui_draw(delta_time, &scene, window, registry, frame_buffer_postprocess);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto size = frame_buffer_postprocess->size();
        auto const& [camera_base, camera] = scene.camera_info();
        renderer_context.projection = glm::perspective(
            glm::radians(camera.fov),
            static_cast<float>(size.x) / static_cast<float>(size.y),
            0.1F,
            100.0F
        );

        // Resize framebuffers according to scene size
        frame_buffer->resize(size);
        frame_buffer_outline->resize(size);
        frame_buffer_identify->resize(size);

        // Render scene
        {
            frame_buffer->bind();

            glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            scene.draw(delta_time);

            systems::draw_light_gizmo(registry);

            frame_buffer->unbind();
        }

        // Render postprocess quad to another frame_buffer
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
            auto mesh_quad = mesh_cache["quad"_hs];
            frame_buffer_postprocess->bind();
            mesh_quad->draw();
            frame_buffer_postprocess->unbind();
            _postprocess_shader->unbind();

            glEnable(GL_DEPTH_TEST);
        }

        // Render outline intermediate buffer
        systems::draw_outline(registry);

        // Render the framebuffer with a unique color per object
        systems::draw_identify(registry);

        ui_end_frame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    });

    terminate_glfw(window);

    return 0;
}
