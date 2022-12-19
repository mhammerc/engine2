#pragma once

#include <string>
#include <filesystem>
#include <optional>

std::optional<std::string> read_file_to_string(const std::filesystem::path &filename);
