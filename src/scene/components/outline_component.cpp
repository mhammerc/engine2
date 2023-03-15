#include "outline_component.h"

#include <entt/entt.hpp>

#include "../../common.h"

using namespace engine;

auto reflection::register_outline_component() -> void {
    auto factory = entt::meta<OutlineComponent>();
    factory.prop("name"_hs, "OutlineComponent");
}