#include "reflection.h"

#include "../components/base_component.h"
#include "../components/camera_component.h"
#include "../components/light_component.h"
#include "../components/material_component.h"
#include "../components/outline_component.h"
#include "../components/skybox_component.h"
#include "transform.h"

using namespace engine;

auto reflection::register_all() -> void {
    reflection::register_camera_component();
    reflection::register_light_component();
    reflection::register_material_component();
    reflection::register_base_component();
    reflection::register_skybox_component();
    reflection::register_outline_component();

    reflection::register_transorm();
}