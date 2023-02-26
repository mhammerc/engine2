#include "component.h"

auto Component::on_start() -> void {}
auto Component::on_update(float /*delta_time*/) -> void {}
auto Component::on_draw() -> void {}

auto Component::game_object() -> GameObject* {
    return _game_object;
}