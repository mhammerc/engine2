#include "load.h"

#include <spdlog/spdlog.h>

#include <entt/entt.hpp>

#include "graphics/deferred_renderer/deferred_renderer_cache.h"
#include "graphics/framebuffer_cache.h"
#include "graphics/mesh_cache.h"
#include "graphics/shader_cache.h"
#include "graphics/texture/texture_cache.h"

using namespace engine;

static auto load_shaders() -> bool {
    auto& shader_cache = entt::locator<engine::ShaderCache>::emplace();

    auto [light_source_shader, _2] = shader_cache.load("light"_hs, ShaderProgram::from_name("light"));
    if (light_source_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [outline_shader, _3] = shader_cache.load("outline"_hs, ShaderProgram::from_name("outline"));
    if (outline_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [postprocess_shader, _4] = shader_cache.load("postprocess"_hs, ShaderProgram::from_name("postprocess"));
    if (postprocess_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [skybox_shader, _7] = shader_cache.load("skybox"_hs, ShaderProgram::from_name("skybox"));
    if (skybox_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [unlit_single_color_shader, _8] =
        shader_cache.load("unlit_single_color"_hs, ShaderProgram::from_name("unlit_single_color"));
    if (unlit_single_color_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [cubemap_viewer_shader, _10] =
        shader_cache.load("cubemap_viewer"_hs, ShaderProgram::from_name("cubemap_viewer"));
    if (cubemap_viewer_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [light_point_shadow_map_shader, _11] =
        shader_cache.load("light_point_shadow_map"_hs, ShaderProgram::from_name("light_point_shadow_map"));
    if (light_point_shadow_map_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [dr_pass_gbuffers_shader, _12] =
        shader_cache.load("dr_pass_gbuffers"_hs, ShaderProgram::from_name("deferred_rendering/dr_pass_gbuffers"));
    if (dr_pass_gbuffers_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [dr_pass_lighting_shader, _13] =
        shader_cache.load("dr_pass_lighting"_hs, ShaderProgram::from_name("deferred_rendering/dr_pass_lighting"));
    if (dr_pass_lighting_shader->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    auto [equirectangular_to_cubemap, _14] =
        shader_cache.load("equirectangular_to_cubemap"_hs, ShaderProgram::from_name("equirectangular_to_cubemap"));
    if (equirectangular_to_cubemap->second.handle() == nullptr) {
        spdlog::critical("could not create shader program.");
        return false;
    }

    return true;
}

static auto load_meshes() -> bool {
    auto& mesh_cache = entt::locator<engine::MeshCache>::value();

    mesh_cache.load("quad"_hs, Mesh::from_quad());
    mesh_cache.load("cube"_hs, Mesh::from_cube());

    return true;
}

static auto load_textures() -> bool {
    auto& texture_cache = entt::locator<engine::TextureCache>::value();

    texture_cache.load("skybox2"_hs, Texture::from_file_cubemap("../assets/skybox2.hdr", Texture::Format::RGBA16F));

    texture_cache.load(
        "skybox"_hs,
        Texture::from_files_cubemap(
            "skybox",
            std::array<std::filesystem::path, 6>({
                "../assets/skybox/right.jpg",
                "../assets/skybox/left.jpg",
                "../assets/skybox/top.jpg",
                "../assets/skybox/bottom.jpg",
                "../assets/skybox/front.jpg",
                "../assets/skybox/back.jpg",
            })
        )
    );

    return true;
}

static auto load_framebuffers() -> bool {
    auto& framebuffer_cache = entt::locator<engine::FramebufferCache>::value();

    using Attachment = Framebuffer::AttachmentDescription;

    std::array<Framebuffer::AttachmentDescription, 2> const attachments = {{
        // Scene (RGB)
        {
            Attachment::Format::RGB,
            Attachment::Type::Texture2D,
            Attachment::TargetBuffer::Color0,
            "Color (RGB)",
        },
        // Identify (X), Unused (GBA)
        {
            Attachment::Format::RGBA16F,
            Attachment::Type::Texture2D,
            Attachment::TargetBuffer::Color1,
            "Identify (X), Unused (GBA)",
        },
    }};

    framebuffer_cache.load("color"_hs, [&]() {
        return Framebuffer::create_with_attachments("color", {1, 1}, attachments);
    });

    return true;
}

auto engine::load_resources() -> bool {
    if (!load_shaders()) {
        return false;
    }

    if (!load_meshes()) {
        return false;
    }

    if (!load_textures()) {
        return false;
    }

    if (!load_framebuffers()) {
        return false;
    }

    return true;
}