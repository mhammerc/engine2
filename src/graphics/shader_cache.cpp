#include "shader_cache.h"

#include <filesystem>

#include "shader_program.h"

using namespace engine;

auto ShaderProgramLoader::operator()(const std::string& name) const -> result_type {
    std::filesystem::path folder = "../shaders";

    auto path = folder / name;

    auto vertex = std::filesystem::path(path) += ".vert";
    auto fragment = std::filesystem::path(path) += ".frag";
    auto geometry = std::filesystem::path(path) += ".geo";

    if (std::filesystem::exists(geometry)) {
        return ShaderProgram::from_vertex_and_fragment_and_geometry(name, vertex, fragment, geometry);
    }

    return ShaderProgram::from_vertex_and_fragment(name, vertex, fragment);
}
