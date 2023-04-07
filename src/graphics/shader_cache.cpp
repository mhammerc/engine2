#include "shader_cache.h"

using namespace engine;

auto ShaderProgramLoader::operator()(std::unique_ptr<ShaderProgram> shader) const -> result_type {
    return shader;
}

auto ShaderProgramLoader::operator()(std::function<std::unique_ptr<ShaderProgram>(void)> const& deferred_loading) const
    -> result_type {
    auto resource = deferred_loading();

    return resource;
}