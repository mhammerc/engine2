#include "platform_info.h"

#include <spdlog/fmt/bundled/core.h>

#include "../opengl/opengl.h"

auto get_platform_info() -> std::string {
    const char* version = (char*)glGetString(GL_VERSION);
    const char* renderer = (char*)glGetString(GL_RENDERER);
    const char* vendor = (char*)glGetString(GL_VENDOR);
    const char* shading_language_version = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    auto platform_info =
        fmt::format("OpenGL {} by {} running on {}. Shaders: {}.", version, vendor, renderer, shading_language_version);

    return platform_info;
}