#include <imgui/imgui.h>
#include <spdlog/fmt/fmt.h>

#include <numeric>

#include "ui_internal.h"

using namespace engine;

auto ui::internal::ui_draw_window_system(float delta_time, GLFWwindow* window) -> void {
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

    ImGui::End();
}