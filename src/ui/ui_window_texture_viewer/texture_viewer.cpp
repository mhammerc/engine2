#include <entt/entt.hpp>
#include <ranges>
#include <vector>

#include "../../common.h"
#include "../../graphics/texture/texture_cache.h"
#include "../ui_internal.h"
#include "ui_window_texture_viewer.h"

using namespace engine;

auto ui::internal::draw_texture_viewer() -> void {
    auto& texture_cache = entt::locator<TextureCache>::value();
    static u32 current_index = static_cast<u32>(-1);

    auto view = std::vector(texture_cache.begin(), texture_cache.end())
        | std::views::transform([](auto const& pair) { return pair.second.handle().get(); });

    auto vector = std::vector(view.begin(), view.end());

    draw_textures(vector, current_index);
}