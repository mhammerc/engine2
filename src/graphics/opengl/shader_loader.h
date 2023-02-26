#pragma once

#include "shader_program.h"

namespace engine {

struct shader_program_loader {
    using result_type = std::shared_ptr<ShaderProgram>;

    auto operator()(std::string const& name) const -> result_type;
};

}  // namespace engine