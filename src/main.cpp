#include <entt/entt.hpp>

#include "components/base_component.h"
#include "components/camera_component.h"
#include "components/outline_component.h"
#include "core/game_loop.h"
#include "core/input.hpp"
#include "core/reflection.h"
#include "create_scene.h"
#include "entt/entity/fwd.hpp"
#include "graphics/deferred_renderer.h"
#include "graphics/deferred_renderer_cache.h"
#include "graphics/framebuffer.h"
#include "graphics/framebuffer_cache.h"
#include "graphics/mesh_cache.h"
#include "graphics/renderer_context.h"
#include "graphics/shader_cache.h"
#include "graphics/shader_program.h"
#include "graphics/texture/texture_cache.h"
#include "load.h"
#include "platform/glfw.h"
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
    entt::locator<engine::ShaderCache>::emplace();
    entt::locator<engine::MeshCache>::emplace();
    entt::locator<engine::FramebufferCache>::emplace();
    entt::locator<engine::DeferredRendererCache>::emplace();
    entt::locator<engine::Input>::emplace();

    load_resources();

    RendererContext renderer_context;
    create_scene(registry, renderer_context);

    engine::game_loop([&](float delta_time, bool& should_quit) {
        if (glfwWindowShouldClose(window) != 0) {
            should_quit = true;
        }

        auto& deferred_renderer_cache = entt::locator<engine::DeferredRendererCache>::value();
        auto& framebuffer_cache = entt::locator<engine::FramebufferCache>::value();
        auto& mesh_cache = entt::locator<engine::MeshCache>::value();
        auto& shader_cache = entt::locator<engine::ShaderCache>::value();

        auto deferred_renderer = deferred_renderer_cache["renderer"_hs];

        Framebuffer& framebuffer = framebuffer_cache["color"_hs];
        Framebuffer& framebuffer_postprocess = framebuffer_cache["postprocess"_hs];

        registry.clear<OutlineComponent>();

        systems::should_close_system();
        systems::camera_system(delta_time, registry);

        ui_prepare_frame();
        ui_draw(delta_time, &renderer_context, window, registry, &framebuffer_postprocess);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto size = framebuffer_postprocess.size();
        auto const& [camera_base, camera] = registry.get<BaseComponent, CameraComponent>(renderer_context.camera);

        renderer_context.projection = glm::perspective(
            glm::radians(camera.fov),
            static_cast<float>(size.x) / static_cast<float>(size.y),
            0.1F,
            100.0F
        );
        renderer_context.view = camera.view_matrix(camera_base);
        renderer_context.camera_transform = camera_base.world_transform(registry);

        // Resize framebuffers according to scene size
        framebuffer.resize(size);

        // Render shadow maps
        systems::draw_shadow_maps(registry);

        if (renderer_context.wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        deferred_renderer
            ->draw({.framebuffer = &framebuffer, .position = {0, 0}, .size = size}, registry, renderer_context);

        if (renderer_context.wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Render scene
        {
            framebuffer.bind();

            systems::draw_skybox(registry, renderer_context);
            systems::draw_light_gizmo(registry, renderer_context);

            framebuffer.unbind();
        }

        // Render postprocess quad to another frame_buffer
        // TODO: refactor the postprocess shader
        {
            glDisable(GL_DEPTH_TEST);

            framebuffer.color()->activate_as(0);
            auto _postprocess_shader = shader_cache["postprocess"_hs];

            _postprocess_shader->set_uniform("screenTexture", 0);

            int post_process = 0;
            if (renderer_context.post_process.inverse) {
                post_process |= RendererContext::PostProcessing::POST_PROCESS_INVERSE;
            }
            if (renderer_context.post_process.black_and_white) {
                post_process |= RendererContext::PostProcessing::POST_PROCESS_GRAYSCALE;
            }
            if (renderer_context.post_process.sepia) {
                post_process |= RendererContext::PostProcessing::POST_PROCESS_SEPIA;
            }
            if (renderer_context.post_process.blur) {
                post_process |= RendererContext::PostProcessing::POST_PROCESS_BLUR;
            }
            if (renderer_context.post_process.sharpen) {
                post_process |= RendererContext::PostProcessing::POST_PROCESS_SHARPEN;
            }
            if (renderer_context.post_process.edge_dectection) {
                post_process |= RendererContext::PostProcessing::POST_PROCESS_EDGE_DETECTION;
            }
            _postprocess_shader->set_uniform("post_process", post_process);

            _postprocess_shader->bind();
            auto mesh_quad = mesh_cache["quad"_hs];
            framebuffer_postprocess.bind();
            mesh_quad->draw();
            framebuffer_postprocess.unbind();
            _postprocess_shader->unbind();

            glEnable(GL_DEPTH_TEST);
        }

        // Render outline intermediate buffer
        systems::draw_outline(registry, renderer_context);

        ui_end_frame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    });

    terminate_glfw(window);

    return 0;
}
