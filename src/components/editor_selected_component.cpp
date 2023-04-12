#include "editor_selected_component.h"

#include <entt/entt.hpp>

#include "../common.h"

using namespace engine;

auto reflection::register_editor_selected_component() -> void {
    auto factory = entt::meta<EditorSelectedComponent>();
    factory.prop("name"_hs, "EditorSelectedComponent");
}