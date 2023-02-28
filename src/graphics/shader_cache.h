#pragma once

#include <entt/entt.hpp>

#include "shader_program.h"

namespace engine {

struct ShaderProgramLoader {
    using result_type = std::shared_ptr<ShaderProgram>;

    auto operator()(std::string const& name) const -> result_type;
};

using ShaderCache = entt::resource_cache<ShaderProgram, ShaderProgramLoader>;

}  // namespace engine