#include "skybox_component.h"

#include <entt/entt.hpp>

#include "../../common.h"

using namespace engine;

auto reflection::register_skybox_component() -> void {
    auto factory = entt::meta<SkyboxComponent>();
    factory.prop("name"_hs, "SkyboxComponent");
}