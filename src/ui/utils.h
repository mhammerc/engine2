#pragma once

#include <imgui/imgui.h>

#include <string>

namespace engine::ui::internal {
auto input_text(
    const char* label,
    std::string* str,
    ImGuiInputTextFlags flags = 0,
    ImGuiInputTextCallback callback = nullptr,
    void* user_data = nullptr
) -> bool;
}