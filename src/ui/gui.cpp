#include "gui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <algorithm>
#include <array>
#include <numeric>

#include "spdlog/fmt/bundled/format.h"

auto gui_init(GLFWwindow* window) -> bool {
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

auto gui_prepare_frame() -> void {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

auto gui_end_frame() -> void {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static auto hierarchy_item(GameObject& object, size_t& currently_selected) -> void {
    const ImGuiTreeNodeFlags tree_base_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    const ImGuiTreeNodeFlags tree_leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    ImGuiTreeNodeFlags flags = tree_base_flags;

    bool is_leaf = object.childrens().empty();

    if (is_leaf) {
        flags |= tree_leaf_flags;
    }

    if (object.unique_index() == currently_selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool is_open = ImGui::TreeNodeEx((void*)(intptr_t)object.unique_index(), flags, "%s", object.name().c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        currently_selected = object.unique_index();
    }

    if (is_open && !is_leaf) {
        auto& childrens = object.childrens();

        std::for_each(childrens.begin(), childrens.end(), [&currently_selected](std::unique_ptr<GameObject>& object) {
            hierarchy_item(*object, currently_selected);
        });
        ImGui::TreePop();
    }
}

auto gui_show_hierarchy(Scene* scene, GLFWwindow* /*window*/) -> void {
    ImGui::SeparatorText("Hierarchy");

    auto& childrens = scene->world.childrens();

    static size_t currently_selected = -1;

    std::for_each(childrens.begin(), childrens.end(), [](std::unique_ptr<GameObject>& object) {
        hierarchy_item(*object, currently_selected);
    });
}

auto gui_show_system_window(Scene* scene, float delta_time, GLFWwindow* window) -> void {
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

    gui_show_hierarchy(scene, window);

    ImGui::End();
}