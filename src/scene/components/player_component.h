#pragma once

namespace engine {
struct PlayerComponent {
    bool flashlight = false;
};

namespace reflection {
    auto register_player_component() -> void;
}
}  // namespace engine