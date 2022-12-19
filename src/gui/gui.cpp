#include "gui.h"
#include "spdlog/fmt/bundled/format.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <array>
#include <numeric>

auto gui_init(GLFWwindow *window) -> bool {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

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

auto gui_show_system_window(float delta_time, GLFWwindow *window) -> void {
  // Create a window called "My First Tool", with a menu bar.
  ImGui::Begin("Engine");

  const auto sample_size = 60;
  static std::array<float, sample_size> last_delta_times{};
  static size_t last_delta_times_index = 0;

  last_delta_times.at(last_delta_times_index) = delta_time;

  last_delta_times_index = (last_delta_times_index + 1) % sample_size;

  auto fps_avg =
	  std::accumulate(last_delta_times.begin(), last_delta_times.end(), 0.F) / static_cast<float>(sample_size);

  auto fps_text = fmt::format("FPS: {:0>3.0f}", 1.F / fps_avg);
  ImGui::Text(fps_text.c_str());

  int width = 0;
  int height = 0;
  glfwGetWindowSize(window, &width, &height);
  auto virtual_resolution_text = fmt::format("Virtual Resolution: {}x{}", width, height);
  ImGui::Text(virtual_resolution_text.c_str());

  glfwGetFramebufferSize(window, &width, &height);
  auto rendered_resolution_text = fmt::format("Rendered Resolution: {}x{}", width, height);
  ImGui::Text(rendered_resolution_text.c_str());

  ImGui::End();
}
