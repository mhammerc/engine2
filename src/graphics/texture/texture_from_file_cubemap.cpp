#include <array>
#include <entt/entt.hpp>

#include "../mesh_cache.h"
#include "../shader_cache.h"
#include "texture.h"

using namespace engine;

// NOLINTBEGIN(cert-err58-cpp)

static const mat4 projection = glm::perspective(glm::radians(90.0F), 1.0F, 0.1F, 10.0F);

// Six camera angles for the six sides of the cubemap
static const std::array<mat4, 6> views = {{
    glm::lookAt(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(1.0F, 0.0F, 0.0F), glm::vec3(0.0F, -1.0F, 0.0F)),
    glm::lookAt(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(-1.0F, 0.0F, 0.0F), glm::vec3(0.0F, -1.0F, 0.0F)),
    glm::lookAt(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 0.0F, 1.0F)),
    glm::lookAt(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, -1.0F, 0.0F), glm::vec3(0.0F, 0.0F, -1.0F)),
    glm::lookAt(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 0.0F, 1.0F), glm::vec3(0.0F, -1.0F, 0.0F)),
    glm::lookAt(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 0.0F, -1.0F), glm::vec3(0.0F, -1.0F, 0.0F)),
}};

// NOLINTEND(cert-err58-cpp)

/**
 * Render the six face of the cubemap.
 */
static auto render_cubemap(u32 framebuffer, vec2i render_size, Texture* cubemap, Texture* equirectangular_projection)
    -> void {
    auto shader = entt::locator<ShaderCache>::value()["equirectangular_to_cubemap"_hs];
    auto cube = entt::locator<MeshCache>::value()["cube"_hs];

    shader->set_uniform("projection", projection);

    equirectangular_projection->activate_as(1, false);
    shader->set_uniform("equirectangular_map", 1);

    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, render_size.x, render_size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    shader->bind();

    for (u32 i = 0; i < 6; ++i) {
        auto const view = views[i];
        shader->set_uniform("view", view);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            cubemap->handle(),
            0
        );

        cube->draw();
    }

    shader->unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    equirectangular_projection->activate_as(1, true);
}

auto Texture::from_file_cubemap(const std::filesystem::path& path, Format format, bool flip)
    -> std::unique_ptr<Texture> {
    PROFILER_PERMANENT_BLOCK("Texture::from_file_cubemap ({})", path.string().c_str());

    // Load the equirectangular projection as a regular OpenGL texture
    auto equirectangular_projection = Texture::from_file_2d(path, format, flip);

    if (!equirectangular_projection) {
        return nullptr;
    }

    // Rough estimate of a cubemap face size
    auto size = vec2i(equirectangular_projection->size().x / 4);

    // Create an empty cubemap
    auto cubemap = Texture::from_empty(path.string(), Type::CubeMap, format, size);

    // Create a framebuffer to render the cubemap on
    u32 framebuffer = 0;
    {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }

    render_cubemap(framebuffer, size, cubemap.get(), equirectangular_projection.get());

    // unbind and delete framebuffer
    {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }

    return cubemap;
}