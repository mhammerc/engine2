#include "reflection.h"

#include "../scene/components/material_component.h"
#include "../scene/components/name_component.h"
#include "../scene/components/skybox_component.h"
#include "../scene/components/transform_component.h"

using namespace engine;

auto reflection::register_all() -> void {
    reflection::register_material_component();
    reflection::register_name_component();
    reflection::register_skybox_component();
    reflection::register_transform_component();
}