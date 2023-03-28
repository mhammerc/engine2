#pragma once

#include <array>
#include <entt/entt.hpp>
#include <memory>
#include <tuple>
#include <vector>

#include "components/base_component.h"
#include "components/camera_component.h"

#define POST_PROCESS_INVERSE (1 << 0)
#define POST_PROCESS_GRAYSCALE (1 << 1)
#define POST_PROCESS_SEPIA (1 << 2)
#define POST_PROCESS_BLUR (1 << 3)
#define POST_PROCESS_SHARPEN (1 << 4)
#define POST_PROCESS_EDGE_DETECTION (1 << 5)

namespace engine {

/**
 * A `Scene` represent the "world". Everything that currently lives into
 * the world such as objects, lights, behaviors and more.
 */
class Scene {
  public:
    Scene(entt::registry& registry);
    auto draw_skybox() -> void;

    entt::registry& registry;

    entt::entity camera;
    [[nodiscard]] auto camera_info() -> std::tuple<BaseComponent&, CameraComponent&>;

    bool wireframe = false;
    bool inverse = false;
    bool black_and_white = false;
    bool sepia = false;
    bool blur = false;
    bool sharpen = false;
    bool edge_dectection = false;
};

}  // namespace engine