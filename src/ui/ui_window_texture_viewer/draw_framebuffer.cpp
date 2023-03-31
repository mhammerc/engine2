#include <ranges>

#include "ui_window_texture_viewer.h"

using namespace engine;

auto ui::internal::draw_framebuffer(Framebuffer* framebuffer, u32& texture_index) -> void {
    auto textures_view = framebuffer->attachments()
        | std::views::transform([](Framebuffer::Attachment const& att) { return att.second.get(); });
    auto textures = std::vector(textures_view.begin(), textures_view.end());

    draw_textures(textures, texture_index);
}