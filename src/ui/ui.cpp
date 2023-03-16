#include "ui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include "../scene/components/outline_component.h"
#include "ui_internal.h"

using namespace engine;

auto engine::ui_init(GLFWwindow* window) -> bool {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;

    auto& style = ImGui::GetStyle();
    style.WindowRounding = 2.F;
    style.ChildRounding = 2.F;
    style.FrameRounding = 2.F;
    style.PopupRounding = 2.F;
    style.GrabRounding = 2.F;

    ImFontConfig config {};
    config.OversampleH = 3;
    config.OversampleV = 3;
    io.Fonts->AddFontFromFileTTF("../assets/JetBrainsMono-Medium.ttf", 16, &config);
    io.Fonts->Build();

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    return true;
}

auto engine::ui_prepare_frame() -> void {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

auto engine::ui_end_frame() -> void {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto engine::ui_draw(
    float delta_time,
    Scene* scene,
    GLFWwindow* window,
    entt::registry& registry,
    FrameBuffer* scene_texture
) -> void {
    ImGui::BeginMainMenuBar();

    static entt::entity selected_entity = entt::null;
    static bool demo_window = false;
    static bool input_debugger = false;
    static bool framebuffer_viewer = false;

    if (ImGui::BeginMenu("Options")) {
        if (ImGui::BeginMenu("Colors")) {
            if (ImGui::MenuItem("Light")) {
                ImGui::StyleColorsLight();
            }

            if (ImGui::MenuItem("Dark")) {
                ImGui::StyleColorsDark();
            }

            if (ImGui::MenuItem("Classic")) {
                ImGui::StyleColorsClassic();
            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Widgets Documentation")) {
            demo_window = true;
        }

        if (ImGui::MenuItem("Input Debugger")) {
            input_debugger = true;
        }

        if (ImGui::MenuItem("Framebuffer Viewer")) {
            framebuffer_viewer = true;
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    ImGui::DockSpaceOverViewport();

    if (demo_window) {
        ImGui::ShowDemoWindow(&demo_window);
    }

    ui::internal::ui_draw_window_input_debugger(&input_debugger);
    ui::internal::ui_draw_window_framebuffer_viewer(&framebuffer_viewer);

    ui::internal::ui_draw_window_system(scene, delta_time, window);
    ui::internal::ui_draw_window_hierarchy(registry, selected_entity);

    if (entt::entity new_selected_entity = ui::internal::ui_draw_window_scene(registry, scene_texture);
        new_selected_entity != entt::null) {
        selected_entity = new_selected_entity;
    }

    ui::internal::ui_draw_window_entity_editor(registry, selected_entity);

    if (selected_entity != entt::null) {
        registry.get_or_emplace<OutlineComponent>(selected_entity);
    }
}
