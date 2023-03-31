#include <entt/entt.hpp>

#include "../../graphics/deferred_renderer/deferred_renderer_cache.h"
#include "../imgui.h"
#include "imgui/imgui.h"
#include "ui_window_texture_viewer.h"

using namespace engine;

static auto draw_renderer_list(u32& current_index) -> void {
    auto& deferred_renderer_cache = entt::locator<DeferredRendererCache>::value();

    auto current = deferred_renderer_cache[current_index];
    auto const preview_name = current ? current->name() : "Please select";

    if (ImGui::BeginCombo("Deferred Renderer", preview_name.c_str())) {
        for (auto deferred_renderer_pair : deferred_renderer_cache) {
            auto [index, deferred_renderer] = deferred_renderer_pair;

            auto selectable_name = fmt::format("{}##{}", deferred_renderer->name(), index);

            if (ImGui::Selectable(selectable_name.c_str(), index == current_index)) {
                current_index = index;
            }
        }

        ImGui::EndCombo();
    }
}

static auto draw_framebuffer_list(DeferredRenderer* renderer) -> Framebuffer* {
    enum class CurrentFramebuffer { Gbuffer, BeforePostProcessing, AfterPostProcessing };

    static auto current_framebuffer = CurrentFramebuffer::Gbuffer;

    char const* gbuffer_name = "G-Buffers";
    char const* before_postprocess_name = "Before Post-Processing";
    char const* after_postprocess_name = "After Post-Processing";

    char const* current_name = gbuffer_name;
    if (current_framebuffer == CurrentFramebuffer::BeforePostProcessing) {
        current_name = before_postprocess_name;
    }
    if (current_framebuffer == CurrentFramebuffer::AfterPostProcessing) {
        current_name = after_postprocess_name;
    }

    if (ImGui::BeginCombo("Framebuffer", current_name)) {
        if (ImGui::Selectable(gbuffer_name)) {
            current_framebuffer = CurrentFramebuffer::Gbuffer;
        }

        if (ImGui::Selectable(before_postprocess_name)) {
            current_framebuffer = CurrentFramebuffer::BeforePostProcessing;
        }

        if (ImGui::Selectable(after_postprocess_name)) {
            current_framebuffer = CurrentFramebuffer::AfterPostProcessing;
        }

        ImGui::EndCombo();
    }

    if (current_framebuffer == CurrentFramebuffer::Gbuffer) {
        return renderer->gbuffers();
    }
    if (current_framebuffer == CurrentFramebuffer::BeforePostProcessing) {
        return renderer->before_post_processing();
    }
    if (current_framebuffer == CurrentFramebuffer::AfterPostProcessing) {
        return renderer->after_post_processing();
    }

    return renderer->gbuffers();
}

auto ui::internal::draw_deferred_renderer_viewer() -> void {
    auto& deferred_renderer_cache = entt::locator<DeferredRendererCache>::value();
    static u32 current_index = static_cast<u32>(-1);

    draw_renderer_list(current_index);

    auto renderer = deferred_renderer_cache[current_index];
    if (!renderer) {
        return;
    }

    auto* framebuffer = draw_framebuffer_list(renderer.handle().get());

    static u32 texture_index = static_cast<u32>(-1);
    draw_framebuffer(framebuffer, texture_index);
}