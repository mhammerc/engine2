#include <entt/entt.hpp>

#include "../../graphics/framebuffer_cache.h"
#include "../../graphics/mesh_cache.h"
#include "../../graphics/shader_cache.h"
#include "../../graphics/texture/texture_internal.h"
#include "../../utils/opengl_handle_to_pointer.h"
#include "../imgui.h"
#include "ui_window_texture_viewer.h"

using namespace engine;

enum UnfoldedOrEquirectangular { Unfolded, Equirectangular };

static auto texture_size_to_be_drawn(Texture* texture) -> vec2 {
    vec2 texture_size = texture->size();
    auto window_size = ImGui::GetContentRegionAvail();

    vec2 display_size;

    if (window_size.x < (window_size.y + 70)) {
        float ratio = texture_size.y / texture_size.x;

        display_size = {window_size.x, window_size.x * ratio};
    } else {
        float ratio = texture_size.x / texture_size.y;

        display_size = {window_size.y * ratio, window_size.y};
    }

    return display_size;
}

static auto draw_texture_2d(Texture* texture, bool reverse_y = true) {
    auto display_size = texture_size_to_be_drawn(texture);
    void* texture_handle = opengl_handle_to_pointer(texture->handle());

    if (!reverse_y) {
        ImGui::Image(texture_handle, display_size, ImVec2(0, 0), ImVec2(1, 1));
    } else {
        ImGui::Image(texture_handle, display_size, ImVec2(0, 1), ImVec2(1, 0));
    }
}

static auto draw_cubemap_mode() -> UnfoldedOrEquirectangular {
    static UnfoldedOrEquirectangular mode = Equirectangular;

    char const* mode_text = mode == Unfolded ? "Unfolded" : "Equirectangular projection";

    if (ImGui::BeginCombo("Mode", mode_text)) {
        if (ImGui::Selectable("Unfolded", mode == Unfolded)) {
            mode = Unfolded;
        }
        if (ImGui::Selectable("Equirectangular projection", mode == Equirectangular)) {
            mode = Equirectangular;
        }

        ImGui::EndCombo();
    }

    return mode;
}

static auto get_framebuffer() -> Framebuffer* {
    auto& framebuffer_cache = entt::locator<FramebufferCache>::value();

    auto framebuffer_insertion = framebuffer_cache.load("cubemap_viewer"_hs, []() {
        const vec2i size = {1024, 1024};

        return Framebuffer::create_with_color("cubemap_viewer", size);
    });

    auto framebuffer_resource = framebuffer_insertion.first->second;
    auto* framebuffer = framebuffer_resource.handle().get();

    return framebuffer;
}

static auto draw_cubemap_to_texture(Texture* cubemap, UnfoldedOrEquirectangular projection) -> Texture* {
    auto* framebuffer = get_framebuffer();
    auto shader = entt::locator<ShaderCache>::value()["cubemap_viewer"_hs];
    auto quad = entt::locator<MeshCache>::value()["quad"_hs];

    framebuffer->clear();

    cubemap->activate_as(0);
    shader->set_uniform("cubemap", 0);
    shader->set_uniform("mode", projection == Unfolded);

    glDisable(GL_DEPTH_TEST);
    framebuffer->bind();
    shader->bind();

    quad->draw();

    shader->unbind();
    framebuffer->unbind();
    glEnable(GL_DEPTH_TEST);

    cubemap->activate_as(0, true);

    return framebuffer->color();
}

static auto draw_texture_cubemap(Texture* texture) {
    auto mode = draw_cubemap_mode();
    auto* projection = draw_cubemap_to_texture(texture, mode);
    draw_texture_2d(projection, false);
}

auto ui::internal::draw_texture(Texture* texture) -> void {
    if (!texture) {
        return;
    }

    auto properties = ::engine::internal::texture::OpenGLTextureProperties::from_texture_format(texture->format());
    auto info = fmt::format("Format: {}. Size: ({},{}).", properties.name, texture->size().x, texture->size().y);

    ImGui::Text("%s", info.c_str());

    if (texture->type() == Texture::Type::Texture2D) {
        draw_texture_2d(texture);
    }

    if (texture->type() == Texture::Type::CubeMap) {
        draw_texture_cubemap(texture);
    }
}