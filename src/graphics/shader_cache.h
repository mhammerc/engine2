#pragma once

#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "../core/noncopyable.h"
#include "shader_program.h"

namespace engine {

/**
 * See TextureLoader for documentation on the loading paradigm.
 */
struct ShaderProgramLoader {
    using result_type = std::shared_ptr<ShaderProgram>;

    auto operator()(std::unique_ptr<ShaderProgram> shader) const -> result_type;

    auto operator()(std::function<std::unique_ptr<ShaderProgram>(void)> const& deferred_loading) const -> result_type;
};

class ShaderCache: public entt::resource_cache<ShaderProgram, ShaderProgramLoader>, public noncopyable {};

}  // namespace engine