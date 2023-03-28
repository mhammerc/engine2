#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace engine {

auto read_file_to_string(const std::filesystem::path& filename) -> std::optional<std::string>;
auto read_glsl_file_to_string(const std::filesystem::path& filename) -> std::optional<std::string>;

}  // namespace engine