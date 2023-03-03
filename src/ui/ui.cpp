#include "ui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    ImGui::DockSpaceOverViewport();

    ImGui::ShowDemoWindow();

    static entt::entity currently_selected = entt::null;

    ui::internal::ui_draw_window_system(scene, delta_time, window);
    ui::internal::ui_draw_window_hierarchy(registry, currently_selected);
    ui::internal::ui_draw_window_scene(scene_texture);
    ui::internal::ui_draw_window_entity_editor(registry, currently_selected);
}
