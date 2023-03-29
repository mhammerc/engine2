#include "outline_component.h"

#include "../common.h"
#include "entt/entt.hpp"

using namespace engine;

auto reflection::register_outline_component() -> void {
    auto factory = entt::meta<OutlineComponent>();
    factory.prop("name"_hs, "OutlineComponent");
}