#include "platform_info.h"

#include <spdlog/fmt/bundled/core.h>

#include "../opengl/opengl.h"

auto get_platform_info() -> std::string {
    const auto* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const auto* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const auto* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const auto* shading_language_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    auto platform_info =
        fmt::format("OpenGL {} by {} running on {}. Shaders: {}.", version, vendor, renderer, shading_language_version);

    return platform_info;
}