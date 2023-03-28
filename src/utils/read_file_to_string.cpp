#include "read_file_to_string.h"

#include <spdlog/spdlog.h>
#include <stb_include/stb_include.h>

#include <array>
#include <fstream>
#include <vector>

auto engine::read_file_to_string(const std::filesystem::path& filename) -> std::optional<std::string> {
    std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    if (fileSize == -1) {
        return std::nullopt;
    }

    std::vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return std::string(bytes.data(), fileSize);
}

auto engine::read_glsl_file_to_string(const std::filesystem::path& filename) -> std::optional<std::string> {
    std::array<char, 256> error {0};

    auto directory = filename;
    directory.remove_filename();

    auto* file = stb_include_file(
        const_cast<char*>(filename.string().c_str()),
        nullptr,
        const_cast<char*>(directory.string().c_str()),
        error.data()
    );

    if (!file) {
        spdlog::error("read_glsl_file_to_string: {}", error.data());
        return std::nullopt;
    }

    std::string str(file);

    free(file);

    return str;
}
