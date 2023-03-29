#include "skybox_component.h"

#include "../common.h"
#include "entt/entt.hpp"

using namespace engine;

auto reflection::register_skybox_component() -> void {
    auto factory = entt::meta<SkyboxComponent>();
    factory.prop("name"_hs, "SkyboxComponent");
}