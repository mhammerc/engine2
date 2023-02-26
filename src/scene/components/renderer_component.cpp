#include "renderer_component.h"

#include "../game_object.h"
#include "material_component.h"
#include "spdlog/spdlog.h"
#include "transform_component.h"

auto RendererComponent::on_draw() -> void {
    spdlog::info("Now drawing {}.", game_object()->name());

    auto* transform = game_object()->component<TransformComponent>();
    auto* material = game_object()->component<MaterialComponent>();

    if (!transform || !material) {
        spdlog::error(
            "[GameObject {}] RendererComponent::on_draw: GameObject must have Transform and Material components."
        );
    }
}