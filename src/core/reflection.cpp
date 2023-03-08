#include "reflection.h"

#include "../scene/components/base_component.h"
#include "../scene/components/camera_component.h"
#include "../scene/components/light_component.h"
#include "../scene/components/material_component.h"
#include "../scene/components/player_component.h"
#include "../scene/components/skybox_component.h"
#include "transform.h"

using namespace engine;

auto reflection::register_all() -> void {
    reflection::register_camera_component();
    reflection::register_light_component();
    reflection::register_material_component();
    reflection::register_base_component();
    reflection::register_player_component();
    reflection::register_skybox_component();

    reflection::register_transorm();
}