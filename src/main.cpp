#include <entt/entt.hpp>

#include "components/outline_component.h"
#include "core/game_loop.h"
#include "core/input.hpp"
#include "core/reflection.h"
#include "create_scene.h"
#include "entt/entity/fwd.hpp"
#include "graphics/deferred_renderer/deferred_renderer_cache.h"
#include "graphics/framebuffer.h"
#include "graphics/framebuffer_cache.h"
#include "graphics/mesh_cache.h"
#include "graphics/shader_cache.h"
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
    entt::locator<engine::ProfilerFrame>::emplace();
    auto& permanent_profiler = entt::locator<engine::ProfilerPermanent>::emplace();

    permanent_profiler.enable_for_next_frame();
    permanent_profiler.start_frame();

    if (!load_resources()) {
        return 1;
    }

    create_scene(registry);

    engine::game_loop([&](float delta_time, bool& should_quit) {
        if (glfwWindowShouldClose(window) != 0) {
            should_quit = true;
        }

        registry.clear<OutlineComponent>();

        auto& framebuffer_cache = entt::locator<engine::FramebufferCache>::value();
        Framebuffer& color = framebuffer_cache["color"_hs];

        entt::locator<ProfilerFrame>::value().start_frame();

        {
            PROFILER_BLOCK("UI");
            ui_prepare_frame();
            ui_draw(delta_time, window, registry, &color);
            ui_end_frame();
        }

        color.clear();
        systems::should_close_system();
        systems::camera_system(delta_time, registry);
        systems::draw_shadow_maps(registry);
        systems::draw_render_deferred(registry);

        entt::locator<ProfilerFrame>::value().end_frame();

        glfwPollEvents();
        glfwSwapBuffers(window);
    });

    terminate_glfw(window);

    return 0;
}
