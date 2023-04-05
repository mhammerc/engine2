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
    enum class CurrentFramebuffer { Gbuffer, BeforePostProcessing, AfterPostProcessing, Downsamples, Upsamples };

    static auto current_framebuffer = CurrentFramebuffer::Gbuffer;

    char const* gbuffer_name = "G-Buffers";
    char const* before_postprocess_name = "Before Post-Processing";
    char const* after_postprocess_name = "After Post-Processing";
    char const* downsamples_name = "Bloom: Downsamples";
    char const* upsamples_name = "Bloom: Upsamples";

    char const* current_name = gbuffer_name;
    if (current_framebuffer == CurrentFramebuffer::BeforePostProcessing) {
        current_name = before_postprocess_name;
    }
    if (current_framebuffer == CurrentFramebuffer::AfterPostProcessing) {
        current_name = after_postprocess_name;
    }
    if (current_framebuffer == CurrentFramebuffer::Downsamples) {
        current_name = downsamples_name;
    }
    if (current_framebuffer == CurrentFramebuffer::Upsamples) {
        current_name = upsamples_name;
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

        if (ImGui::Selectable(downsamples_name)) {
            current_framebuffer = CurrentFramebuffer::Downsamples;
        }

        if (ImGui::Selectable(upsamples_name)) {
            current_framebuffer = CurrentFramebuffer::Upsamples;
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
    if (current_framebuffer == CurrentFramebuffer::Downsamples) {
        static u32 current_downsample_index = 0;
        auto const& downsamples = renderer->downsamples();

        if (current_downsample_index >= downsamples.size()) {
            current_downsample_index = 0;
        }

        auto* current_downsample = downsamples[current_downsample_index].get();

        if (ImGui::BeginCombo("Downsample", current_downsample->name().c_str())) {
            for (u32 i = 0; i < downsamples.size(); ++i) {
                auto* fb = downsamples[i].get();

                if (ImGui::Selectable(fb->name().c_str())) {
                    current_downsample_index = i;
                }
            }

            ImGui::EndCombo();
        }

        return renderer->downsamples()[current_downsample_index].get();
    }
    if (current_framebuffer == CurrentFramebuffer::Upsamples) {
        static u32 current_upsample_index = 0;
        auto const& upsamples = renderer->upsamples();

        if (current_upsample_index >= upsamples.size()) {
            current_upsample_index = 0;
        }

        auto* current_upsample = upsamples[current_upsample_index].get();

        if (ImGui::BeginCombo("Upsample", current_upsample->name().c_str())) {
            for (u32 i = 0; i < upsamples.size(); ++i) {
                auto* fb = upsamples[i].get();

                if (ImGui::Selectable(fb->name().c_str())) {
                    current_upsample_index = i;
                }
            }

            ImGui::EndCombo();
        }

        return renderer->upsamples()[current_upsample_index].get();
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