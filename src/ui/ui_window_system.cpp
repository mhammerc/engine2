#include <imgui/imgui.h>
#include <spdlog/fmt/fmt.h>

#include <numeric>

#include "ui_internal.h"

using namespace engine;

auto ui::internal::ui_draw_window_system(Scene* scene, float delta_time, GLFWwindow* window) -> void {
    ImGui::Begin("Engine");

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

    ImGui::SeparatorText("Lighting");
    ImGui::Checkbox("Flashlight", &scene->flashlight);

    ImGui::SeparatorText("Shaders");
    ImGui::Checkbox("Wireframe", &scene->wireframe);
    ImGui::Checkbox("Objects Outline", &scene->outline);
    ImGui::Checkbox("Show Normals", &scene->show_normals);
    ImGui::SliderFloat("Explosion", &scene->explosion, 0.F, 1.F);

    ImGui::SeparatorText("Post-Processing");
    ImGui::Checkbox("Inverse", &scene->inverse);
    ImGui::Checkbox("Grayscale", &scene->black_and_white);
    ImGui::Checkbox("Sepia", &scene->sepia);
    ImGui::Checkbox("Blur", &scene->blur);
    ImGui::Checkbox("Sharpen", &scene->sharpen);
    ImGui::Checkbox("Edge Detection", &scene->edge_dectection);

    ImGui::End();
}