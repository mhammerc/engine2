#pragma once

#include <array>
#include <span>

#include "../common.h"

namespace engine {
struct Input {
  public:
    Input();
    ~Input() = default;

    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;

    Input(Input&&) noexcept = default;
    auto operator=(Input&&) noexcept -> Input&;

    static auto get_key_name(int key) -> char const*;

    auto on_key(int key, bool pressed) -> void;
    auto on_mouse_position(vec2 position) -> void;

  private:
    std::array<bool, 1024> _keys {};
    vec2 _mouse_position = vec2(0.F, 0.F);

  public:
    std::span<bool, 1024> const keys;
    [[nodiscard]] auto mouse_position() const -> vec2;
};
}  // namespace engine
