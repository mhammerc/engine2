#pragma once

#include "../core/resource_cache.h"
#include "shader_program.h"

namespace engine {

struct ShaderProgramLoader {
    using result_type = std::shared_ptr<ShaderProgram>;

    auto operator()(std::string const& name) const -> result_type;
};

using ShaderCache = engine::resource_cache<ShaderProgram, ShaderProgramLoader>;

}  // namespace engine