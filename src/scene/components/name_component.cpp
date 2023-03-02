#include "name_component.h"

#include <entt/entt.hpp>

#include "../../common.h"

using namespace engine;

auto reflection::register_name_component() -> void {
    auto factory = entt::meta<NameComponent>();
    factory.prop("name"_hs, "NameComponent");

    factory.data<&NameComponent::name>("name"_hs).prop("name"_hs, "name");
    factory.data<&NameComponent::enabled>("enabled"_hs).prop("name"_hs, "enabled");
}