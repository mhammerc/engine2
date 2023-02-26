#pragma once

class GameObject;

class Component {
  public:
    Component() = default;
    virtual ~Component() noexcept = default;

    Component(const Component&) = default;
    auto operator=(const Component&) -> Component& = default;

    Component(Component&&) noexcept = default;
    auto operator=(Component&&) noexcept -> Component& = default;

  protected:
    friend class GameObject;

    virtual auto on_start() -> void;
    virtual auto on_update(float delta_time) -> void;
    virtual auto on_draw() -> void;

    auto game_object() -> GameObject*;

  private:
    GameObject* _game_object = nullptr;
};