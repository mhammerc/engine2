#include "player_component.h"

#include <entt/entt.hpp>

#include "../../common.h"

using namespace engine;

auto reflection::register_player_component() -> void {
    auto factory = entt::meta<PlayerComponent>();
    factory.prop("name"_hs, "PlayerComponent");

    factory.data<&PlayerComponent::flashlight>("flashlight"_hs).prop("name"_hs, "flashlight");
}