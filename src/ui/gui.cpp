#include "gui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <algorithm>
#include <array>
#include <numeric>

#include "../scene/components/name_component.h"
#include "spdlog/fmt/bundled/format.h"

using namespace engine;

auto engine::gui_init(GLFWwindow* window) -> bool {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    return true;
}

auto engine::gui_prepare_frame() -> void {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

auto engine::gui_end_frame() -> void {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static auto hierarchy_item(entt::entity entity, NameComponent const& name, entt::entity& currently_selected) -> void {
    const ImGuiTreeNodeFlags tree_base_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    const ImGuiTreeNodeFlags tree_leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    ImGuiTreeNodeFlags flags = tree_base_flags;

    // bool is_leaf = object.childrens().empty();
    bool is_leaf = true;

    if (is_leaf) {
        flags |= tree_leaf_flags;
    }

    if (entity == currently_selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool is_open = ImGui::TreeNodeEx((void*)(intptr_t)entity, flags, "%s", name.name.c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        currently_selected = entity;
    }

    if (is_open && !is_leaf) {
        // auto& childrens = object.childrens();

        // std::for_each(childrens.begin(), childrens.end(), [&currently_selected](std::unique_ptr<GameObject>& object) {
        //     hierarchy_item(*object, currently_selected);
        // });
        ImGui::TreePop();
    }
}

static auto gui_show_hierarchy(Scene* /*scene*/, GLFWwindow* /*window*/, entt::registry& registry) -> void {
    ImGui::SeparatorText("Hierarchy");

    static entt::entity currently_selected = entt::null;

    auto view = registry.view<NameComponent>();

    for (auto [entity, name] : view.each()) {
        hierarchy_item(entity, name, currently_selected);
    }
}

auto engine::gui_show_system_window(Scene* scene, float delta_time, GLFWwindow* window, entt::registry& registry)
    -> void {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    // window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, main_viewport->Size.y), ImGuiCond_Always);

    ImGui::Begin("Engine", nullptr, window_flags);
    ImGui::SeparatorText("System");

    const auto sample_size = 120;
    static std::array<float, sample_size> last_delta_times {};
    static size_t last_delta_times_index = 0;

    last_delta_times.at(last_delta_times_index) = delta_time;

    last_delta_times_index = (last_delta_times_index + 1) % sample_size;

    auto average_frame_time =
        std::accumulate(last_delta_times.begin(), last_delta_times.end(), 0.F) / static_cast<float>(sample_size);

    auto fps_text = fmt::format("Average Frame Time: {:.0f}ms", average_frame_time * 1000);
    ImGui::Text("%s", fps_text.c_str());

    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);
    ImGui::Text("Virtual Resolution: %dx%d", width, height);

    glfwGetFramebufferSize(window, &width, &height);
    ImGui::Text("Rendered Resolution: %dx%d", width, height);

    if (ImGui::CollapsingHeader("Lighting")) {
        ImGui::Checkbox("Flashlight", &scene->flashlight);
    }

    if (ImGui::CollapsingHeader("Shaders")) {
        ImGui::Checkbox("Wireframe", &scene->wireframe);

        ImGui::Checkbox("Mirror", &scene->mirror);

        ImGui::Checkbox("Glass", &scene->glass);

        ImGui::Checkbox("Objects Outline", &scene->outline);

        ImGui::Checkbox("Show Normals", &scene->show_normals);

        ImGui::SliderFloat("Explosion", &scene->explosion, 0.F, 1.F);
    }

    if (ImGui::CollapsingHeader("Post-Processing")) {
        ImGui::Checkbox("Inverse", &scene->inverse);

        ImGui::Checkbox("Grayscale", &scene->black_and_white);

        ImGui::Checkbox("Sepia", &scene->sepia);

        ImGui::Checkbox("Blur", &scene->blur);

        ImGui::Checkbox("Sharpen", &scene->sharpen);

        ImGui::Checkbox("Edge Detection", &scene->edge_dectection);
    }

    gui_show_hierarchy(scene, window, registry);

    ImGui::End();
}
