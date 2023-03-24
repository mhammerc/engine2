
#include <spdlog/fmt/fmt.h>

#include <entt/entt.hpp>
#include <optional>

#include "../../common.h"
#include "../../graphics/framebuffer_cache.h"
#include "../../graphics/mesh_cache.h"
#include "../../graphics/shader_cache.h"
#include "../../graphics/texture/texture_cache.h"
#include "../imgui.h"
#include "../ui_internal.h"
#include "ui_window_texture_viewer.h"

using namespace engine;
using namespace ui::internal;

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

static auto draw_selector(u32& current_index) -> void {
    auto& texture_cache = entt::locator<TextureCache>::value();

    auto current = texture_cache[current_index];
    auto const preview_name = current ? current->name() : "Please select";

    if (ImGui::BeginCombo("Texture", preview_name.c_str())) {
        for (auto texture_pair : texture_cache) {
            auto [index, texture] = texture_pair;

            if (texture->type() != Texture::Type::CubeMap) {
                continue;
            }

            auto selectable_name = fmt::format("{}##{}", texture->name(), index);

            if (ImGui::Selectable(selectable_name.c_str(), index == current_index)) {
                current_index = index;
            }
        }

        ImGui::EndCombo();
    }
}

static auto draw_mode() -> UnfoldedOrEquirectangular {
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

auto ui::internal::draw_cubemap_to_framebuffer(Texture& cubemap, UnfoldedOrEquirectangular mode) -> Framebuffer* {
    auto* framebuffer = get_framebuffer();
    auto shader = entt::locator<ShaderCache>::value()["cubemap_viewer"_hs];
    auto quad = entt::locator<MeshCache>::value()["quad"_hs];

    glDisable(GL_DEPTH_TEST);

    framebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT);

    shader->set_uniform("cubemap", 0);
    shader->set_uniform("mode", mode == Unfolded);

    cubemap.activate_as(0);
    shader->bind();

    quad->draw();

    shader->unbind();
    cubemap.activate_as(0, true);

    framebuffer->unbind();

    glEnable(GL_DEPTH_TEST);

    return framebuffer;
}

auto ui::internal::draw_cubemap_viewer() -> void {
    auto& texture_cache = entt::locator<TextureCache>::value();
    static u32 current_index = static_cast<u32>(-1);

    draw_selector(current_index);
    auto mode = draw_mode();

    if (current_index == static_cast<u32>(-1)) {
        return;
    }

    auto cubemap = texture_cache[current_index];
    auto* framebuffer = draw_cubemap_to_framebuffer(cubemap, mode);
    auto* texture = framebuffer->color();
    auto* texture_handle = reinterpret_cast<void*>(texture->handle());

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

    ImGui::Image(texture_handle, display_size, ImVec2(0, 0), ImVec2(1, 1));
}