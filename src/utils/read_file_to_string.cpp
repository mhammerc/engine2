#include "read_file_to_string.h"

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
